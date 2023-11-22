 -- 1) Написать функцию, возвращающую таблицу TransferredPoints в более человекочитаемом виде

CREATE OR REPLACE FUNCTION fnc_transferred_points()
    RETURNS table
            (
                peer1        varchar,
                peer2        varchar,
                PointsAmount bigint
            )
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN QUERY
        WITH agg AS
                 (SELECT t1.id                                          AS id1,
                         t2.id                                          AS id2,
                         t1.checkingpeer                                AS peer_1,
                         t1.checkedpeer                                 AS peer_2,
                         t1.pointsamount - coalesce(t2.pointsamount, 0) AS points_amount
                  FROM transferredpoints AS t1
                           LEFT JOIN transferredpoints AS t2
                                     ON t1.checkingpeer = t2.checkedpeer
                                         AND t1.checkedpeer = t2.checkingpeer)
        SELECT peer_1, peer_2, points_amount
        FROM agg
        WHERE id2 IS NULL
        UNION
        (SELECT a.peer_1, a.peer_2, agg.points_amount
         FROM (SELECT id1 * id2 AS multi, max(peer_1) AS peer_1, min(peer_2) AS peer_2
               FROM agg
               WHERE id2 IS NOT NULL
               GROUP BY id1 * id2) AS a
                  JOIN agg ON a.peer_1 = agg.peer_1 AND a.peer_2 = agg.peer_2)
        ORDER BY peer_1, peer_2;
END;
$$;

SELECT *
FROM fnc_transferred_points();


-- 2) Написать функцию, которая возвращает таблицу вида: ник пользователя, название проверенного задания, кол-во полученного XP
-- В таблицу включать только задания, успешно прошедшие проверку (определять по таблице Checks).
-- Одна задача может быть успешно выполнена несколько раз. В таком случае в таблицу включать все успешные проверки.

CREATE OR REPLACE FUNCTION fnc_checks_xp()
    RETURNS TABLE
            (
                peer varchar,
                task varchar,
                xp   integer
            )
AS
$$
SELECT c.peer, regexp_replace(c.task, '_.*', ''), xp.xpamount AS xp
FROM checks c
         JOIN p2p p on c.id = p."Check"
         JOIN xp on c.id = xp."Check"
WHERE p.state = 'Success'
ORDER BY 1, 2, 3;
$$
    LANGUAGE sql;

SELECT *
FROM fnc_checks_xp();

-- 3) Написать функцию, определяющую пиров, которые не выходили из кампуса в течение всего дня

CREATE OR REPLACE FUNCTION fnc_full_day_peers(date)
    RETURNS TABLE
            (
                peer varchar
            )
AS
$$
WITH tmp AS
         (SELECT peer, date, COUNT(state) AS count
          FROM timetracking
          GROUP BY peer, date)

SELECT DISTINCT peer
FROM tmp
WHERE date = $1
  AND count = 2;
$$
    LANGUAGE sql;

SELECT *
FROM fnc_full_day_peers('2023-03-15');


-- 4) Найти процент успешных и неуспешных проверок за всё время

CREATE OR REPLACE PROCEDURE prc_success_failure_checks(ref REFCURSOR)
    LANGUAGE plpgsql AS
$$
BEGIN
    OPEN ref FOR
        WITH all_checks AS
                 (SELECT p2p."Check", p2p.state AS ps, v.state AS vs
                  FROM p2p
                           JOIN verter v on p2p."Check" = v."Check"
                  WHERE p2p.state = 'Success'
                     OR p2p.state = 'Failure')
        SELECT ((SELECT count(ps)
                 FROM all_checks
                 WHERE ps = 'Success'
                   AND vs != 'Failure') * 100 / (SELECT count(*) FROM all_checks)) AS "SuccessfulChecks",
               ((SELECT count(ps)
                 FROM all_checks
                 WHERE ps = 'Failure'
                    OR vs = 'Failure') * 100 / (SELECT count(*) FROM all_checks))  AS "UnsuccessfulChecks";
END;
$$;

BEGIN;
CALL prc_success_failure_checks('ref');
FETCH ALL IN "ref";
COMMIT;


-- 5) Посчитать изменение в количестве пир поинтов каждого пира по таблице TransferredPoints
-- Результат вывести отсортированным по изменению числа поинтов.
-- Формат вывода: ник пира, изменение в количество пир поинтов

CREATE OR REPLACE PROCEDURE prc_count_points(ref REFCURSOR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        WITH union_table AS
                 (SELECT checkingpeer AS Peer, pointsamount
                  FROM transferredpoints
                  UNION ALL
                  SELECT checkedpeer, pointsamount * (-1)
                  FROM transferredpoints)

        SELECT Peer, SUM(pointsamount) AS "PointsChange"
        FROM union_table
        GROUP BY Peer
        ORDER BY 2 DESC;
END;
$$;

BEGIN;
CALL prc_count_points('ref');
FETCH ALL IN "ref";
COMMIT;

-- 6) Посчитать изменение в количестве пир поинтов каждого пира по таблице,
-- возвращаемой первой функцией из Part 3

CREATE OR REPLACE PROCEDURE prc_count_points(ref REFCURSOR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        SELECT agg.peer, sum(agg.points_change) AS "PointsChange"
        FROM (SELECT peer1 AS peer, PointsAmount AS points_change
              FROM fnc_transferred_points()
              UNION ALL
              SELECT peer2 AS peer, PointsAmount * (-1) AS points_change
              FROM fnc_transferred_points()) AS agg
        GROUP BY agg.peer
        ORDER BY "PointsChange" DESC, agg.peer;
END;
$$;

BEGIN;
CALL prc_count_points('ref');
FETCH ALL IN "ref";
COMMIT;

-- 7) Определить самое часто проверяемое задание за каждый день

CREATE OR REPLACE FUNCTION fnc_max_count_checked_task_of_day(target_date date)
    RETURNS int
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN (SELECT count(*) AS count
            FROM checks
            GROUP BY date, task
            HAVING date = target_date
            ORDER BY count DESC
            LIMIT 1);
END;
$$;

CREATE OR REPLACE PROCEDURE prc_most_popular_task_of_day(ref REFCURSOR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        WITH agg AS (SELECT date, task, count(*) AS count
                     FROM checks
                     GROUP BY date, task
                     ORDER BY date DESC)
        SELECT to_char(date, 'DD.MM.YYYY') AS day, substring(task from '^(.+?)_') AS task
        FROM agg
        WHERE count = fnc_max_count_checked_task_of_day(agg.date);
END;
$$;

BEGIN;
CALL prc_most_popular_task_of_day('ref');
FETCH ALL IN "ref";
COMMIT;

-- 8) Определить длительность последней P2P проверки
-- Под длительностью подразумевается разница
-- между временем, указанным в записи со статусом "начало",
-- и временем, указанным в записи со статусом "успех" или "неуспех".
-- Формат вывода: длительность проверки

CREATE OR REPLACE PROCEDURE prc_finished_time(ref REFCURSOR)
    LANGUAGE plpgsql AS
$$
BEGIN
    OPEN ref FOR
        WITH cte_finished_time AS (SELECT id, "Check", "Time"
                                   FROM p2p
                                   WHERE state = 'Success'
                                      OR state = 'Failure'
                                   ORDER BY 1 DESC
                                   LIMIT 1)
        SELECT to_char((c."Time" - (SELECT "Time"
                                    FROM p2p
                                    WHERE "Check" = c."Check"
                                      AND state = 'Start')), 'HH24:MI:SS') as "CheckDuration"
        FROM cte_finished_time c;
END;
$$;

BEGIN;
CALL prc_finished_time('ref');
FETCH ALL IN "ref";
COMMIT;


-- 9) Найти всех пиров, выполнивших весь заданный блок задач и дату завершения
-- последнего задания

CREATE OR REPLACE PROCEDURE prc_peer_who_completed_all_tasks_from_block(IN block_name VARCHAR, IN ref REFCURSOR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        WITH tmp AS (SELECT *
                     FROM Tasks
                     WHERE Title SIMILAR TO CONCAT(block_name, '[0-7]%')),
             check_task AS (SELECT MAX(title) AS Title FROM tmp),
             check_date AS (SELECT c.peer, c.task, c.date
                            FROM Checks c
                                     JOIN P2P ON c.Id = P2P."Check"
                            WHERE P2P.State = 'Success')
        SELECT check_date.Peer AS Peer, TO_CHAR(date, 'dd.mm.yyyy') AS Day
        FROM check_date
                 JOIN check_task ON check_date.Task = check_task.Title;
END;
$$;

BEGIN;
CALL prc_peer_who_completed_all_tasks_from_block('SQL', 'ref');
FETCH ALL IN "ref";
COMMIT;


-- 10) Определить, к какому пиру стоит идти на проверку каждому обучающемуся
-- Определять нужно исходя из рекомендаций друзей пира, т.е. нужно найти пира,
-- проверяться у которого рекомендует наибольшее число друзей.
-- Формат вывода: ник пира, ник найденного проверяющего

CREATE OR REPLACE PROCEDURE prc_recommended_from_friends(ref REFCURSOR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        WITH agg AS (SELECT f.peer1, r.RecommendedPeer, count(*) AS count
                     FROM friends f
                              JOIN Recommendations r
                                   ON f.peer2 = r.peer AND f.peer1 <> r.RecommendedPeer
                     GROUP BY f.peer1, r.RecommendedPeer)
        SELECT a1.peer1 AS peer, min(a1.recommendedpeer) AS "RecommendedPeerr"
        FROM agg AS a1
        WHERE a1.count = (SELECT max(a2.count) FROM agg AS a2 WHERE a1.peer1 = a2.peer1)
        GROUP BY a1.peer1
        ORDER BY a1.peer1;
END;
$$;

BEGIN;
CALL prc_recommended_from_friends('ref');
FETCH ALL IN "ref";
COMMIT;


-- 11) Определить процент пиров, которые:
-- Приступили к блоку 1
-- Приступили к блоку 2
-- Приступили к обоим
-- Не приступили ни к одному


CREATE OR REPLACE PROCEDURE prc_percent_of_peers(ref REFCURSOR, block1 varchar, block2 varchar)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        WITH count_of_peers AS (SELECT count(*) FROM peers),
             peers_stat AS
                 (SELECT nickname,
                         count(DISTINCT CASE WHEN task SIMILAR TO format('%s[0-9]+_%%', block1) THEN 1 END) AS b1,
                         count(DISTINCT CASE WHEN task SIMILAR TO format('%s[0-9]+_%%', block2) THEN 1 END) AS b2
                  FROM peers p
                           LEFT JOIN checks c on p.nickname = c.peer
                  GROUP BY nickname)
        SELECT round(count(CASE WHEN b1 = 1 AND b2 = 0 THEN 1 END)::numeric / (SELECT * FROM count_of_peers) * 100, 0)
                   AS "StartedBlock1",
               round(count(CASE WHEN b1 = 0 AND b2 = 1 THEN 1 END)::numeric / (SELECT * FROM count_of_peers) * 100, 0)
                   AS "StartedBlock2",
               round(count(CASE WHEN b1 = 1 AND b2 = 1 THEN 1 END)::numeric / (SELECT * FROM count_of_peers) * 100, 0)
                   AS "StartedBothBlocks",
               round(count(CASE WHEN b1 = 0 AND b2 = 0 THEN 1 END)::numeric / (SELECT * FROM count_of_peers) * 100, 0)
                   AS "DidntStartAnyBlock"
        FROM peers_stat;
END;
$$;

BEGIN;
CALL prc_percent_of_peers('ref', 'CPP', 'A');
FETCH ALL IN "ref";
COMMIT;



-- 12) Определить N пиров с наибольшим числом друзей
-- Параметры процедуры: количество пиров N.
-- Результат вывести отсортированным по кол-ву друзей.
-- Формат вывода: ник пира, количество друзей


CREATE OR REPLACE PROCEDURE prc_max_friends(ref REFCURSOR, N integer)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        WITH all_peers AS
                 (SELECT peer1 AS peer, count(peer2) AS friendsCount
                  FROM (SELECT peer1, peer2
                        FROM friends
                        UNION ALL
                        SELECT peer2, peer1
                        FROM friends) AS twice_friends
                  GROUP BY peer)

        SELECT nickname AS peer, COALESCE(friendsCount, 0) as "FriendsCount"
        FROM all_peers
                 RIGHT JOIN peers ON nickname = peer
        ORDER BY 2 DESC, 1
        LIMIT N;
END;
$$;

BEGIN;
CALL prc_max_friends('ref', 4);
FETCH ALL IN "ref";
COMMIT;


-- 13) Определить процент пиров, которые когда-либо успешно проходили проверку в свой день рождения
-- Также определите процент пиров, которые хоть раз проваливали проверку в свой день рождения.
-- Формат вывода: процент успехов в день рождения, процент неуспехов в день рождения


CREATE OR REPLACE PROCEDURE prc_find_peer_percent(ref REFCURSOR)
    LANGUAGE plpgsql AS
$$
BEGIN
    OPEN ref FOR
        WITH checks_on_birthday AS (SELECT nickname, date, p2p.state AS p_state, v.state AS v_state
                                    FROM peers p
                                             JOIN checks c ON p.nickname = c.peer
                                             JOIN p2p ON c.id = p2p."Check"
                                             LEFT JOIN verter v ON c.id = v."Check" AND
                                                                   (v.state = 'Success' OR v.state = 'Failure')
                                    WHERE date_part('day', c.date) = date_part('day', p.birthday)
                                      AND date_part('month', c.date) = date_part('month', p.birthday)
                                      AND (p2p.state = 'Success' OR p2p.state = 'Failure')),
             agg_table AS (SELECT nickname,
                                  date,
                                  (
                                      CASE
                                          WHEN p_state = 'Success' AND v_state <> 'Failure'
                                              THEN 1
                                          ELSE 0
                                          END
                                      ) AS final_state
                           FROM checks_on_birthday)

        SELECT ((SELECT sum(final_state)
                 FROM agg_table) * 100 / (SELECT count(*) FROM agg_table))       AS "SuccessfulChecks",
               (100 - (SELECT sum(final_state)
                       FROM agg_table) * 100 / (SELECT count(*) FROM agg_table)) AS "UnsuccessfulChecks";
END;
$$;

BEGIN;
CALL prc_find_peer_percent('ref');
FETCH ALL IN "ref";
COMMIT;



-- 14) Определить кол-во XP, полученное в сумме каждым пиром
-- Если одна задача выполнена несколько раз, полученное за нее кол-во XP равно максимальному за эту задачу.
-- Результат вывести отсортированным по кол-ву XP.
-- Формат вывода: ник пира, количество XP


CREATE OR REPLACE PROCEDURE prc_find_sum_xp(ref REFCURSOR)
    LANGUAGE plpgsql AS
$$
BEGIN
    OPEN ref FOR
        WITH max_xp AS (SELECT c.peer, MAX(table_xp.xpamount) AS max_xp
                        FROM checks c
                                 JOIN xp AS table_xp ON c.id = table_xp."Check"
                        GROUP BY c.peer, task)
        SELECT m.peer AS Peer, SUM(max_xp) AS XP
        FROM max_xp m
        GROUP BY m.peer
        ORDER BY XP;
END;
$$;

BEGIN;
CALL prc_find_sum_xp('ref');
FETCH ALL IN "ref";
COMMIT;

-- 15) Определить всех пиров, которые сдали заданные задания 1 и 2, но не сдали задание 3
-- Параметры процедуры: названия заданий 1, 2 и 3.
-- Формат вывода: список пиров


CREATE OR REPLACE PROCEDURE prc_peer_checker(ref REFCURSOR, task1 VARCHAR, task2 VARCHAR, task3 VARCHAR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        WITH agg AS (SELECT c.id,
                            c.peer         AS checked,
                            p.checkingpeer AS checking,
                            c.task,
                            p.state        AS p_state,
                            v.state        AS v_state
                     FROM checks c
                              JOIN p2p p ON c.id = p."Check" AND c.task IN (task1, task2, task3) AND p.state = 'Success'
                              LEFT JOIN verter v on c.id = v."Check"
                     WHERE v.state = 'Success'
                        OR v.state IS NULL)
        SELECT a1.checked AS peer
        FROM agg AS a1
        WHERE a1.task = task1
        INTERSECT
        SELECT a2.checked AS peer
        FROM agg AS a2
        WHERE a2.task = task2
        EXCEPT
        SELECT a3.checked AS peer
        FROM agg AS a3
        WHERE a3.task = task3;
END;
$$;

BEGIN;
CALL prc_peer_checker('ref', 'A1_Maze', 'SQL2_Info21_v1.0', 'CPP3_SmartCalc_v2.0');
FETCH ALL IN "ref";
COMMIT;


-- 16) Используя рекурсивное обобщенное табличное выражение, для каждой задачи вывести кол-во предшествующих ей задач
-- То есть сколько задач нужно выполнить, исходя из условий входа, чтобы получить доступ к текущей.
-- Формат вывода: название задачи, количество предшествующих

CREATE OR REPLACE PROCEDURE prc_count_previous_task(IN ref REFCURSOR)
    LANGUAGE plpgsql AS
$$
BEGIN
    OPEN ref FOR
        WITH RECURSIVE recursion
                           AS (SELECT 'C2_SimpleBashUtils'::varchar AS task,
                                      0::bigint                     AS "PrevCount"
                               UNION
                               SELECT title, recursion."PrevCount" + 1
                               FROM recursion,
                                    tasks
                               WHERE parenttask = recursion.task
                                 AND "PrevCount" < (select count(*) from tasks))
        SELECT regexp_replace(task, '_.*', '') AS task, "PrevCount"
        FROM recursion;
END;
$$;

BEGIN;
CALL prc_count_previous_task('ref');
FETCH ALL IN "ref";
COMMIT;


-- 17) Найти "удачные" для проверок дни. День считается "удачным", если в нем есть хотя бы N идущих подряд успешных проверки
-- Параметры процедуры: количество идущих подряд успешных проверок N.
-- Временем проверки считать время начала P2P этапа.
-- Под идущими подряд успешными проверками подразумеваются успешные проверки, между которыми нет неуспешных.
-- При этом кол-во опыта за каждую из этих проверок должно быть не меньше 80% от максимального.
-- Формат вывода: список дней


CREATE OR REPLACE PROCEDURE prc_success_days(ref refcursor, n int)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        SELECT fnc_success_days AS "LuckyDays" FROM fnc_success_days(n);
END;
$$;


CREATE OR REPLACE FUNCTION fnc_success_days(n int)
    RETURNS SETOF date
    LANGUAGE plpgsql
AS
$$
DECLARE
    row  record;
    row2 record;
    i    int := 0;
BEGIN
    FOR row IN
        SELECT c.date FROM checks AS c GROUP BY c.date ORDER BY c.date
        LOOP
            FOR row2 IN
                SELECT *
                FROM (SELECT c.id             AS id,
                             c.date,
                             p2."Time"        AS time,
                             (CASE
                                  WHEN v.state = 'Success' AND x.xpamount * 100 / t.maxxp >= 80 THEN 1
                                  ELSE 0 END) AS state
                      FROM checks c
                               JOIN p2p p on c.id = p."Check" AND p.state <> 'Start'
                               LEFT JOIN verter v on c.id = v."Check" AND v.state <> 'Start'
                               LEFT JOIN xp x on c.id = x."Check"
                               JOIN tasks t on c.task = t.title
                               JOIN p2p p2 on c.id = p2."Check" AND p2.state = 'Start') AS agg
                WHERE row.date = agg.date
                ORDER BY agg.time

                LOOP
                    IF row2.state = 1 THEN
                        i := i + 1;
                        IF i = n THEN
                            RETURN NEXT row2.date;
                            EXIT;
                        END IF;
                    ELSE
                        i := 0;
                    END IF;
                END LOOP;
            i := 0;
        END LOOP;
    RETURN;
END;
$$;

BEGIN;
CALL prc_success_days('ref', 2);
FETCH ALL IN "ref";
COMMIT;


-- 18) Определить пира с наибольшим числом выполненных заданий
-- Формат вывода: ник пира, число выполненных заданий


-- DROP PROCEDURE prc_find_peer_max_success;

CREATE OR REPLACE PROCEDURE prc_find_peer_max_success(ref REFCURSOR)
    LANGUAGE plpgsql AS
$$
BEGIN
    OPEN ref FOR
        SELECT c.peer AS Peer, count(*) AS "CompletedTasksCount "
        FROM checks c
                 JOIN p2p p on c.id = p."Check" AND p.state = 'Success'
                 LEFT JOIN verter v on c.id = v."Check" AND v.state = 'Success'
        GROUP BY c.peer
        ORDER BY 2 DESC
        LIMIT 1;
END;
$$;

BEGIN;
CALL prc_find_peer_max_success('ref');
FETCH ALL IN "ref";
COMMIT;

-- 19) Определить пира с наибольшим количеством XP

CREATE OR REPLACE PROCEDURE prc_find_peer_max_xp(ref REFCURSOR)
    LANGUAGE plpgsql AS
$$
BEGIN
    OPEN ref FOR
        SELECT c.peer AS Peer, SUM(xp.xpamount) AS XP
        FROM xp
                 JOIN checks c ON (xp."Check" = c.id)
        GROUP BY c.peer
        ORDER BY 2 DESC
        LIMIT 1;
END;
$$;


BEGIN;
CALL prc_find_peer_max_xp('ref');
FETCH ALL IN "ref";
COMMIT;



-- 20) Определить пира, который провел сегодня в кампусе больше всего времени
-- Формат вывода: ник пира

DROP PROCEDURE IF EXISTS prc_longest_interval CASCADE;
CREATE OR REPLACE PROCEDURE prc_longest_interval(OUT "PeerName" varchar) AS
$$
BEGIN
    WITH time_in AS (SELECT peer, SUM(time) AS time_in_campus
                     FROM timetracking
                     WHERE date = current_date
                       AND state = '1'
                     GROUP BY peer),
         time_out AS (SELECT peer, SUM(time) AS time_out_campus
                      FROM timetracking
                      WHERE date = current_date
                        AND state = '2'
                      GROUP BY peer),
         diff_time AS (SELECT time_in.peer, (time_out_campus - time_in_campus) AS full_time
                       FROM time_in
                                JOIN time_out ON time_in.peer = time_out.peer)
    SELECT peer
    INTO "PeerName"
    FROM diff_time
    ORDER BY full_time DESC
    LIMIT 1;
END;
$$ LANGUAGE plpgsql;

BEGIN;
CALL prc_longest_interval('ref');
COMMIT;


-- 21) Определить пиров, приходивших раньше заданного времени не менее N раз за всё время
-- Параметры процедуры: время, количество раз N.
-- Формат вывода: список пиров


DROP PROCEDURE IF EXISTS prc_peer_came_early CASCADE;
CREATE OR REPLACE PROCEDURE prc_peer_came_early(IN checktime time, IN N int, IN ref refcursor) AS
$$
BEGIN
    OPEN ref FOR
        SELECT peer
        FROM (SELECT peer, MIN(time) AS min_time, date
              FROM timetracking
              WHERE state = '1'
              GROUP BY date, peer) AS t
        WHERE min_time < checktime
        GROUP BY peer
        HAVING COUNT(peer) >= N;
END;
$$ LANGUAGE plpgsql;

BEGIN;
CALL prc_peer_came_early('19:00:00', 1, 'ref');
FETCH ALL IN "ref";
END;


-- 22) Определить пиров, выходивших за последние N дней из кампуса больше M раз
-- Параметры процедуры: количество дней N, количество раз M.
-- Формат вывода: список пиров


CREATE OR REPLACE PROCEDURE prc_peers_who_out_of_campus_more_then_M(ref REFCURSOR, N int, M int)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        SELECT t.peer
        FROM peers p
                 JOIN timetracking t ON p.nickname = t.peer AND t.state = '2' AND t.date > now()::date - N
        GROUP BY t.peer
        HAVING count(t.time) - count(DISTINCT t.date) > M;
END;
$$;


BEGIN;
CALL prc_peers_who_out_of_campus_more_then_M('ref', 15, 0);
FETCH ALL IN "ref";
COMMIT;

-- 23) Определить пира, который пришел сегодня последним
-- Формат вывода: ник пира


CREATE OR REPLACE PROCEDURE prc_peer_who_came_today_last(ref REFCURSOR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        SELECT peer
        FROM timetracking
        WHERE date = now()::date
          AND state = '1'
        GROUP BY peer
        ORDER BY min(time) DESC
        LIMIT 1;
END;
$$;

BEGIN;
CALL prc_peer_who_came_today_last('ref');
FETCH ALL IN "ref";
COMMIT;

-- 24) Определить пиров, которые выходили вчера из кампуса больше чем на N минут
-- Параметры процедуры: количество минут N.
-- Формат вывода: список пиров


CREATE OR REPLACE PROCEDURE prc_peers_who_came_out_yesterday_more_then_N_minutes(ref REFCURSOR, N interval minute)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        SELECT fnc_peers_who_came_out_yesterday_more_then_N_minutes AS peer_name
        FROM fnc_peers_who_came_out_yesterday_more_then_N_minutes(N);
END;
$$;


CREATE OR REPLACE FUNCTION fnc_peers_who_came_out_yesterday_more_then_N_minutes(N interval minute)
    RETURNS SETOF varchar
    LANGUAGE plpgsql
AS
$$
DECLARE
    peer_name     varchar;
    row           record;
    time_out      time;
    is_first_step bool := TRUE;

BEGIN
    FOR peer_name IN
        SELECT peer
        FROM timetracking
        WHERE date = now()::date - 1
        GROUP BY peer
        HAVING count(*) >= 4
        LOOP

            FOR row IN
                SELECT *
                FROM timetracking
                WHERE date = now()::date - 1
                  AND peer = peer_name
                LOOP

                    IF row.state = '2' THEN
                        time_out := row.time;
                    ELSE
                        IF is_first_step = FALSE THEN
                            IF row.time - time_out > N THEN
                                RETURN NEXT peer_name;
                                EXIT;
                            END IF;
                        END IF;
                    END IF;
                    is_first_step := FALSE;

                END LOOP;

            is_first_step := TRUE;
        END LOOP;
    RETURN;
END;
$$;


BEGIN;
CALL prc_peers_who_came_out_yesterday_more_then_N_minutes('ref', '00:20');
FETCH ALL IN "ref";
COMMIT;


-- 25) Определить для каждого месяца процент ранних входов
-- Для каждого месяца посчитать, сколько раз люди, родившиеся в этот месяц,
--      приходили в кампус за всё время (будем называть это общим числом входов).
-- Для каждого месяца посчитать, сколько раз люди, родившиеся в этот месяц,
--      приходили в кампус раньше 12:00 за всё время (будем называть это числом ранних входов).
-- Для каждого месяца посчитать процент ранних входов в кампус относительно общего числа входов.
-- Формат вывода: месяц, процент ранних входов

CREATE OR REPLACE FUNCTION fnc_month_varchar(n int)
    RETURNS varchar
    LANGUAGE plpgsql
AS
$$
DECLARE
    month varchar[] := '{"January", "February", "March", "April", "May",' ||
                       '"June", "July", "August", "September", "October", "November", "December"}';
BEGIN
    RETURN month[n];
END;
$$;

CREATE OR REPLACE FUNCTION fnc_percent_early_entries_for_each_months()
    RETURNS table
            (
                Month        varchar,
                EarlyEntries numeric
            )
    LANGUAGE plpgsql
AS
$$
BEGIN
    FOR curr_month IN 1..12
        LOOP
            RETURN QUERY
                WITH agg AS (SELECT peer,
                                    date,
                                    MIN(time)                                       AS time_min,
                                    CASE WHEN MIN(time) < '12:00' THEN 1 ELSE 0 END AS early
                             FROM timetracking
                             WHERE peer IN (SELECT nickname FROM peers WHERE DATE_PART('month', birthday) = curr_month)
                               AND state = '1'
                             GROUP BY peer, date)
                SELECT fnc_month_varchar(curr_month) AS Month,
                       (SELECT coalesce(round((CASE WHEN sum(early) = 0 THEN 1 ELSE sum(early) END)::numeric /
                                              (CASE WHEN count(*) = 0 THEN 1 ELSE count(*) END) * 100), 0)
                        FROM agg)                    AS "EarlyEntries";

            curr_month := curr_month + 1;
        END LOOP;
END;
$$;


CREATE OR REPLACE PROCEDURE prc_percent_early_entries_for_each_months(ref REFCURSOR)
    LANGUAGE plpgsql
AS
$$
BEGIN
    OPEN ref FOR
        SELECT * FROM fnc_percent_early_entries_for_each_months();
END;
$$;

BEGIN;
CALL prc_percent_early_entries_for_each_months('ref');
FETCH ALL IN "ref";
COMMIT;
