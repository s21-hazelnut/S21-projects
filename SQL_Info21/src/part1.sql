-- CREATE DATABASE info_s21;

-- Переключись на базу "info_s21"
DROP SCHEMA IF EXISTS schema_s21 CASCADE;

-- CREATE ROLE role_s21 WITH LOGIN CREATEDB CREATEROLE;

CREATE SCHEMA IF NOT EXISTS schema_s21 AUTHORIZATION role_s21;
SET search_path = "schema_s21";

create table Tasks
(
    Title      varchar default null primary key,
    ParentTask varchar,
    MaxXP      bigint,
    constraint fk_tasks_tasks_title foreign key (parenttask) references Tasks (title)
);

create table Peers
(
    Nickname varchar not null primary key,
    Birthday date
);

create table checks
(
    id   BIGSERIAL not null primary key,
    Peer varchar,
    Task varchar,
    Date date,
    constraint fk_checks_tasks_title_id foreign key (Task) references Tasks (Title),
    constraint fk_checks_peers_Nickname foreign key (Peer) references peers (Nickname)
);

create table P2P
(
    ID           BIGSERIAL not null primary key,
    "Check"      bigint,
    CheckingPeer varchar,
    State        varchar CHECK (State IN ('Start', 'Success', 'Failure')),
    "Time"        time,
    constraint fk_p2p_checks_check_id foreign key ("Check") references checks (id),
    constraint fk_p2p_peers_Nickname foreign key (CheckingPeer) references peers (Nickname)
);

create table TransferredPoints
(
    id           BIGSERIAL not null primary key,
    CheckedPeer  varchar,
    CheckingPeer varchar,
    PointsAmount bigint,
    constraint fk_TransferredPoints_peers_Nickname1 foreign key (CheckedPeer) references peers (Nickname),
    constraint fk_TransferredPoints_peers_Nickname2 foreign key (CheckingPeer) references peers (Nickname)
);

create table Friends
(
    id    BIGSERIAL not null primary key,
    Peer1 varchar,
    Peer2 varchar,
    constraint fk_Friends_peers_Nickname1 foreign key (Peer1) references peers (Nickname),
    constraint fk_Friends_peers_Nickname2 foreign key (Peer2) references peers (Nickname)
);

create table Recommendations
(
    id               BIGSERIAL not null primary key,
    Peer             varchar,
    RecommendedPeer varchar,
    constraint fk_Recommendations_peers_Nickname1 foreign key (Peer) references peers (Nickname),
    constraint fk_Recommendations_peers_Nickname2 foreign key (RecommendedPeer) references peers (Nickname)
);

create table TimeTracking
(
    id    BIGSERIAL not null primary key,
    Peer  varchar,
    Date  date,
    Time  time,
    State varchar CHECK (State IN ('1', '2')),
    constraint fk_TimeTracking_peers_Nickname1 foreign key (Peer) references peers (Nickname)
);

create table Verter
(
    id      BIGSERIAL not null primary key,
    "Check" bigint,
    State   varchar CHECK (State IN ('Start', 'Success', 'Failure')),
    Time    time,
    constraint fk_Verter_checks_check_id foreign key ("Check") references checks (id)
);

create table XP
(
    id       BIGSERIAL not null primary key,
    "Check"  bigint,
    XPAmount bigint,
    constraint fk_xp_checks_check_id foreign key ("Check") references checks (id)
);


CREATE OR REPLACE PROCEDURE import(
    table_name VARCHAR(50),
    source VARCHAR(100),
    delimiter VARCHAR(1) DEFAULT ',')
    LANGUAGE plpgsql
AS
$$
BEGIN
    EXECUTE format('COPY %I FROM %L WITH DELIMITER %L CSV HEADER', table_name, source, delimiter);
END;
$$;

CALL import('tasks', '/tmp/data1/Tasks.csv');
CALL import('peers', '/tmp/data1/peers.csv');
CALL import('checks', '/tmp/data1/checks.csv');
CALL import('friends', '/tmp/data1/friends.csv');
CALL import('p2p', '/tmp/data1/p2p.csv');
CALL import('timetracking', '/tmp/data1/TimeTracking.csv');
CALL import('recommendations', '/tmp/data1/recommendations.csv');
CALL import('xp', '/tmp/data1/XP.csv');
CALL import('verter', '/tmp/data1/Verter.csv');
CALL import('transferredpoints', '/tmp/data1/TransferredPoints.csv');


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

-- CALL export('tasks', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/tasks1.csv');
-- CALL export('peers', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/peers1.csv');
-- CALL export('checks', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/checks1.csv');
-- CALL export('friends', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/friends1.csv');
-- CALL export('p2p', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/p2p1.csv');
-- CALL export('timetracking', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/timetracking1.csv');
-- CALL export('recommendations', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/recommendations1.csv');
-- CALL export('xp', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/XP1.csv');
-- CALL export('verter', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/verter1.csv');
-- CALL export('transferredpoints', '/Users/hazelnut/SQL2_Info21_v1.0-0/SRC/export/transferredpoints1.csv');
