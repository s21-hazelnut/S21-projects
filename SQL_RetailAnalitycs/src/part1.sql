DROP SCHEMA IF EXISTS schema_s21 CASCADE;
-- CREATE ROLE role_s21 SUPERUSER CREATEDB CREATEROLE;

CREATE SCHEMA IF NOT EXISTS schema_s21 AUTHORIZATION role_s21;
SET search_path = "schema_s21";

SET datestyle TO 'ISO, DMY';

CREATE TABLE Personal_information
(
    Customer_ID            BIGINT  NOT NULL PRIMARY KEY,
    Customer_Name          VARCHAR,
    Customer_Surname       VARCHAR,
    Customer_Primary_Email VARCHAR NOT NULL,
    Customer_Primary_Phone VARCHAR NOT NULL,
--     CONSTRAINT uk_Personal_information_Customer_ID unique (Customer_ID),
--     CONSTRAINT uk_Personal_information_Customer_Primary_Email unique (Customer_Primary_Email),
--     CONSTRAINT uk_Personal_information_Customer_Primary_Phone unique (Customer_Primary_Phone),
    CONSTRAINT ch_Customer_Name CHECK (Customer_Name ~ '(([A-Z]{1}([a-z]||-)*)|([А-ЯЁ]{1}([а-яё]||-)*))'),
    CONSTRAINT ch_Customer_Surname CHECK (Customer_Surname ~ '(([A-Z]{1}([a-z]||-)*)|([А-ЯЁ]{1}([а-яё]||-)*))'),
    CONSTRAINT ch_Customer_Primary_Email CHECK (Customer_Primary_Email ~
                                                '([a-zA-Z0-9_\-\.\+]+)@([a-zA-Z0-9_\-\.]+)\.([a-zA-Z0-9]{2,5})'),
    CONSTRAINT ch_Customer_Primary_Phone CHECK (Customer_Primary_Phone ~ '\+7\d{10}')
);

CREATE TABLE Cards
(
    Customer_Card_ID BIGINT NOT NULL PRIMARY KEY,
    Customer_ID      BIGINT NOT NULL,
    CONSTRAINT fk_Cards_Personal_information_Customer_ID FOREIGN KEY (Customer_ID) REFERENCES Personal_information (Customer_ID)
);

CREATE TABLE SKU_group
(
    Group_ID   BIGINT  NOT NULL PRIMARY KEY,
    Group_Name VARCHAR NOT NULL,
    CONSTRAINT ch_Group_Name CHECK (Group_Name ~
                                    '(([A-Za-zА-ЯЁа-яё]||\-\_\±\!\@\#\$\(\%\^\&\*\)\+\\\|\?\>\<\"\;\:\[\]\{\}\`\~)*)')
);

CREATE TABLE Product_grid
(
    SKU_ID   BIGINT  NOT NULL PRIMARY KEY,
    SKU_Name VARCHAR NOT NULL,
    Group_ID BIGINT  NOT NULL,
    CONSTRAINT fk_Product_grid_SKU_group_Group_ID FOREIGN KEY (Group_ID) REFERENCES SKU_group (Group_ID),
    CONSTRAINT ch_SKU_Name CHECK (SKU_Name ~
                                  '(([A-Za-zА-ЯЁа-яё]||\-\_\±\!\@\#\$\(\%\^\&\*\)\+\\\|\?\>\<\"\;\:\[\]\{\}\`\~)*)')
);

CREATE TABLE Stores
(
    Transaction_Store_ID BIGINT  NOT NULL,
    SKU_ID               BIGINT  NOT NULL,
    SKU_Purchase_Price   NUMERIC NOT NULL,
    SKU_Retail_Price     NUMERIC NOT NULL,
    CONSTRAINT fk_Stores_Product_grid_SKU_ID FOREIGN KEY (SKU_ID) REFERENCES Product_grid (SKU_ID)
);

CREATE TABLE Transactions
(
    Transaction_ID       BIGINT    NOT NULL PRIMARY KEY,
    Customer_Card_ID     BIGINT    NOT NULL,
    Transaction_Summ     NUMERIC   NOT NULL,
    Transaction_DateTime TIMESTAMP NOT NULL,
    Transaction_Store_ID BIGINT    NOT NULL,
    CONSTRAINT uk_Transactions_Transaction_ID unique (Transaction_ID),
    CONSTRAINT fk_Transactions_Cards_Customer_Card_ID FOREIGN KEY (Customer_Card_ID) REFERENCES Cards (Customer_Card_ID)
--     CONSTRAINT fk_Transactions_Stores_Transaction_Store_ID FOREIGN KEY (Transaction_Store_ID) REFERENCES Stores (Transaction_Store_ID)
);

CREATE TABLE Checks
(
    Transaction_ID BIGINT  NOT NULL,
    SKU_ID         BIGINT  NOT NULL,
    SKU_Amount     NUMERIC NOT NULL,
    SKU_Summ       NUMERIC NOT NULL,
    SKU_Summ_Paid  NUMERIC NOT NULL,
    SKU_Discount   NUMERIC NOT NULL,
    CONSTRAINT fk_Checks_Transactions_Transaction_ID FOREIGN KEY (Transaction_ID) REFERENCES Transactions (Transaction_ID),
    CONSTRAINT fk_Checks_Product_grid_SKU_ID FOREIGN KEY (SKU_ID) REFERENCES Product_grid (SKU_ID)
);

CREATE TABLE Date_of_analysis_formation
(
    Analysis_Formation TIMESTAMP NOT NULL
);



CREATE OR REPLACE PROCEDURE import(
    table_name VARCHAR(50),
    source VARCHAR(100),
    delimiter VARCHAR(10) DEFAULT ',')
    LANGUAGE plpgsql
AS
$$
BEGIN
    EXECUTE format('COPY %I FROM %L WITH DELIMITER E''\t''', table_name, source, delimiter);
END;
$$;

CALL import('personal_information', '/tmp/data2/Personal_Data.tsv');
CALL import('cards', '/tmp/data2/Cards.tsv');
CALL import('sku_group', '/tmp/data2/Groups_SKU.tsv');
CALL import('product_grid', '/tmp/data2/SKU.tsv');
CALL import('stores', '/tmp/data2/Stores.tsv');
CALL import('transactions', '/tmp/data2/Transactions.tsv');
CALL import('checks', '/tmp/data2/Checks.tsv');
CALL import('date_of_analysis_formation',
            '/tmp/data2/Date_Of_Analysis_Formation.tsv');



CREATE OR REPLACE PROCEDURE export(
    table_name VARCHAR(50),
    source VARCHAR(100),
    delimiter VARCHAR(5) DEFAULT ',')
    LANGUAGE plpgsql
AS
$$
BEGIN
    EXECUTE format('COPY %I TO %L WITH DELIMITER %L CSV HEADER', table_name, source, delimiter);
END;
$$;

-- CALL export('personal_information',
--             '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_Personal_information.csv');
-- CALL export('cards', '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_Cards.csv');
-- CALL export('product_grid', '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_Product_grid.csv');
-- CALL export('sku_group', '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_SKU_group.csv');
-- CALL export('stores', '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_Stores.csv');
-- CALL export('transactions', '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_Transactions.csv');
-- CALL export('checks', '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_Checks.csv');
-- CALL export('date_of_analysis_formation',
--             '/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src/export/EXP_Date_of_analysis_formation.csv');
