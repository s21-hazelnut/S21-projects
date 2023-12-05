SET search_path = "schema_s21";
DO
$CREATE_roles$
    DECLARE
        db_name     text := (SELECT current_database());
        schema_name text := (SELECT CURRENT_SCHEMA);

    BEGIN
        CREATE ROLE visitor;
        EXECUTE 'GRANT CONNECT ON DATABASE ' || db_name || ' TO visitor';
        EXECUTE 'GRANT USAGE ON SCHEMA ' || schema_name || ' TO visitor';
        EXECUTE 'GRANT SELECT ON ALL TABLES IN SCHEMA ' || schema_name || ' TO visitor';
        EXECUTE 'ALTER DEFAULT PRIVILEGES IN SCHEMA ' || schema_name || ' GRANT SELECT ON TABLES to visitor';

        CREATE ROLE administrator SUPERUSER CREATEDB CREATEROLE;
    END
$CREATE_roles$;

SET role = 'administrator';
show role;

SET role = 'visitor';
show role;


-- ok for visitor and administrator
SELECT *
FROM personal_information;

-- ok for administrator, fail for visitor
CREATE TABLE test_table AS
SELECT *
FROM personal_information;
INSERT INTO test_table (customer_id, customer_name, customer_surname, customer_primary_email, customer_primary_phone)
VALUES ((SELECT max(customer_id) + 1 FROM test_table), 'Валерий', 'Жмышенко', 'zadwd@main.ru', '+78003353535');
UPDATE test_table
SET customer_name='Пожилой'
WHERE customer_id = (SELECT max(customer_id) FROM test_table);
DELETE
FROM test_table
WHERE customer_id = (SELECT max(customer_id) FROM test_table);
DROP TABLE test_table;

SET role = 'role_s21';

DO
$DROP_roles$
    DECLARE
        db_name text := (SELECT current_database());
    BEGIN
        --drop role visitor
        EXECUTE 'REASSIGN OWNED BY visitor TO ' || db_name || '';
        DROP OWNED BY visitor;
        DROP ROLE visitor;

--drop role administrator
        EXECUTE 'REASSIGN OWNED BY administrator TO ' || db_name || '';
        DROP OWNED BY administrator;
        DROP ROLE administrator;
    END
$DROP_roles$;
