-- 1) Написать процедуру добавления P2P проверки
-- Параметры: ник проверяемого, ник проверяющего, название задания, статус P2P проверки, время.
-- Если задан статус "начало", добавить запись в таблицу Checks (в качестве даты использовать сегодняшнюю).
-- Добавить запись в таблицу P2P.
-- Если задан статус "начало", в качестве проверки указать только что добавленную запись, иначе указать проверку с незавершенным P2P этапом.

CREATE OR REPLACE PROCEDURE add_p2p_check(checked varchar, checking varchar,
                                          task_name varchar, ins_state varchar, ins_time time)
    LANGUAGE plpgsql
AS
$$
DECLARE
    count_p2p_check INT;
    MAX_ID_checks   BIGINT := (Select count(*)
                               FROM Checks);
    MAX_ID_p2p      BIGINT := (Select count(*)
                               FROM p2p);
BEGIN
    IF (checked <> checking AND is_available_task(checked, task_name)) THEN
        count_p2p_check := get_count_p2p(checked, checking, task_name);
        IF (ins_state = 'Start') THEN
            IF count_p2p_check % 2 = 0 THEN
                INSERT INTO checks(id, peer, task, date) VALUES (MAX_ID_checks + 1, checked, task_name, now()::date);
                INSERT INTO p2p(id, "Check", checkingpeer, state, "Time")
                VALUES (MAX_ID_p2p + 1, MAX_ID_checks + 1,
                        checking, ins_state, ins_time);
            ELSE
                RAISE EXCEPTION '!!!ACHTUNG!!! This task is already in the start status';
            END IF;

        ELSE
            IF count_p2p_check % 2 = 1 THEN
                INSERT INTO p2p(id, "Check", checkingpeer, state, "Time")
                VALUES (MAX_ID_p2p + 1,
                        get_check_id_start_for_checked_and_checking_and_task(checked, checking, task_name),
                        checking, ins_state, ins_time);
            ELSE
                RAISE EXCEPTION '!!!ACHTUNG!!! This task is not start status';
            END IF;
        END IF;
    ELSIF checked = checking THEN
        RAISE EXCEPTION '!!!ACHTUNG!!! Checked peer and checking peer not should be same';
    ELSE
        RAISE EXCEPTION '!!!ACHTUNG!!! This task is not available for check, because not complete parent task';
    END IF;
END;
$$;


CREATE OR REPLACE FUNCTION is_available_task(checked varchar, task_name varchar)
    RETURNS BOOLEAN
    LANGUAGE plpgsql
AS
$$
DECLARE
    id_for_check BIGINT;
BEGIN
    IF exist_parent_task(task_name) THEN
        id_for_check := get_max_check_id_from_checks_for_checked_and_task(checked,
                                                                          (SELECT parenttask FROM tasks WHERE title = task_name));
        IF (id_for_check = 0 AND is_success_verter_check_task(id_for_check) = FALSE) THEN
            RAISE EXCEPTION '!!!ACHTUNG!!! This task is not available, because parent task was not completed or not success';
        END IF;
    END IF;
    RETURN TRUE;
END;
$$;


CREATE OR REPLACE FUNCTION get_count_p2p(checked varchar, checking varchar, task_name varchar)
    RETURNS INT
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN (SELECT count(*)
            FROM (SELECT * FROM p2p WHERE checkingpeer = checking) AS p
                     JOIN checks c
                          ON p."Check" = c.id
                              AND c.peer = checked
                              AND c.task = task_name);

END;
$$;


CREATE OR REPLACE FUNCTION exist_parent_task(task_name varchar)
    RETURNS BOOLEAN
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN (SELECT parenttask FROM tasks WHERE title = task_name) IS NOT NULL;
END;
$$;


CREATE OR REPLACE FUNCTION get_max_check_id_from_checks_for_checked_and_task(checked varchar, task_name varchar)
    RETURNS BIGINT
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN coalesce(
            (SELECT max(id)
             FROM checks
             WHERE peer = checked
               AND task = task_name),
            0);
END;
$$;


CREATE OR REPLACE FUNCTION get_check_id_start_for_checked_and_checking_and_task(checked varchar, checking varchar, task_name varchar)
    RETURNS BIGINT
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN (SELECT c.id
            FROM (SELECT *
                  FROM p2p
                  WHERE checkingpeer = checking
                    AND p2p.state = 'Start') AS p
                     JOIN checks c
                          ON p."Check" = c.id
                              AND c.peer = checked AND c.task = task_name
            ORDER BY c.date DESC, p."Time" DESC
            LIMIT 1);
END;
$$;


CREATE OR REPLACE FUNCTION is_success_verter_check_task(id_for_check BIGINT)
    RETURNS BOOLEAN
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN (SELECT id
            FROM verter
            WHERE "Check" = id_for_check
              AND state = 'Success') IS NOT NULL;
END;
$$;


-- 2) Написать процедуру добавления проверки Verter'ом
-- Параметры: ник проверяемого, название задания, статус проверки Verter'ом, время.
-- Добавить запись в таблицу Verter (в качестве проверки указать проверку соответствующего задания с самым поздним (по времени) успешным P2P этапом)

CREATE OR REPLACE PROCEDURE add_verter_check(checked varchar, task_name varchar, state varchar, "time" time)
    LANGUAGE plpgsql
AS
$$
BEGIN
    IF is_success_last_p2p_check_task(checked, task_name) THEN
        INSERT INTO verter (id, "Check", state, time)
        VALUES (((SELECT MAX(id) FROM verter) + 1), get_check_id_last_success_p2p_check(checked, task_name), state, time);
    ELSE
        RAISE EXCEPTION '!!!ACHTUNG!!! This task is not success in last p2p check';
    END IF;
END;
$$;


CREATE OR REPLACE FUNCTION is_success_last_p2p_check_task(checked varchar, task_name varchar)
    RETURNS BOOLEAN
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN (SELECT c.id
            FROM (SELECT id
                  FROM checks
                  WHERE peer = checked
                    AND task = task_name
                  ORDER BY id DESC
                  LIMIT 1) AS c
                     JOIN p2p AS p
                          ON c.id = p."Check" AND p.state = 'Success') IS NOT NULL;
END;
$$;


CREATE OR REPLACE FUNCTION get_check_id_last_success_p2p_check(checked varchar, task_name varchar)
    RETURNS BIGINT
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN (SELECT c.id
            FROM (SELECT *
                  FROM checks
                  WHERE peer = checked
                    AND task = task_name
                  ORDER BY id DESC
                  LIMIT 1) AS c
                     JOIN p2p AS p
                          ON c.id = p."Check" AND state = 'Success'
            ORDER BY date DESC, "Time" DESC
            LIMIT 1);
END;
$$;


-- 3) Написать триггер: после добавления записи со статутом "начало" в таблицу P2P, изменить соответствующую запись в таблице TransferredPoints

CREATE OR REPLACE FUNCTION fnc_true_or_false(
    check_id BIGINT,
    fnc_checking_peer VARCHAR(30))
    RETURNS bool
    LANGUAGE plpgsql AS
$$
BEGIN
    RETURN (SELECT count(*) <> 0
            FROM transferredPoints
            WHERE checkedpeer = (SELECT peer
                                 FROM checks
                                 WHERE id = check_id)
              AND checkingpeer = fnc_checking_peer);
END;
$$;

CREATE OR REPLACE FUNCTION fnc_check_transferred_points()
    RETURNS trigger
    LANGUAGE plpgsql AS
$$
BEGIN
    IF (SELECT fnc_true_or_false(NEW."Check", NEW.checkingpeer)) THEN
        UPDATE transferredPoints
        SET pointsamount = pointsamount + 1
        WHERE checkingpeer = NEW.checkingpeer
          AND checkedpeer = (SELECT peer
                             FROM checks
                             WHERE id = NEW."Check");
    ELSE
        INSERT INTO transferredPoints (id, checkingpeer, checkedpeer, pointsamount)
        VALUES (((SELECT MAX(id) FROM transferredPoints) + 1), NEW.checkingpeer,
                (SELECT peer
                 FROM checks
                 WHERE id = NEW."Check"),
                1);
    END IF;
    RETURN null;
END;
$$;


CREATE OR REPLACE TRIGGER trg_after_insert_p2p_success
    AFTER INSERT
    ON p2p
    FOR EACH ROW
    WHEN (NEW.state = 'Start')
EXECUTE PROCEDURE fnc_check_transferred_points();


-- 4) Написать триггер: перед добавлением записи в таблицу XP, проверить корректность добавляемой записи

CREATE OR REPLACE FUNCTION fnc_check_max_xp() RETURNS trigger
    LANGUAGE plpgsql AS
$$
BEGIN
    IF NEW."Check" <> (SELECT "Check" FROM xp WHERE xp."Check" = NEW."Check" LIMIT 1) THEN
        IF NEW.xpamount >
           (SELECT t.maxxp
            FROM checks
                     JOIN tasks t on checks.task = t.title
            WHERE checks.id = NEW."Check")
        THEN
            RAISE EXCEPTION '!!!ACHTUNG!!! Bad argument xpAmount: % > max xp', NEW.xpamount;
        END IF;
    ELSE
        RAISE EXCEPTION '!!!ACHTUNG!!! check already taken into account';
END IF;
    RETURN NEW;
END;
$$;


CREATE OR REPLACE TRIGGER trg_before_insert_xp
    BEFORE INSERT
    ON xp
    FOR EACH ROW
EXECUTE FUNCTION fnc_check_max_xp();

CALL add_p2p_check('ninetale', 'barberry', 'A2_SimpleNavigator', 'Start', '15:25:00');
CALL add_p2p_check('ninetale', 'barberry', 'A2_SimpleNavigator', 'Success', '15:45:00');
CALL add_verter_check('ninetale', 'A2_SimpleNavigator', 'Success', '16:56:00');