SET search_path = "schema_s21";
CREATE OR REPLACE VIEW customers_transactions_summ AS
SELECT pi.customer_id, AVG(t.transaction_summ) AS Customer_Average_Check
FROM transactions t
         LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
         JOIN personal_information pi ON pi.customer_id = c.customer_id
GROUP BY pi.customer_id
ORDER BY 2 DESC;

CREATE OR REPLACE VIEW customers_transactions_summ_with_segment AS
SELECT cts.customer_id,
       cts.Customer_Average_Check,
       CASE
           WHEN row_number() OVER (ORDER BY Customer_Average_Check DESC) <= count(*) OVER () * 0.1 THEN 'High'
           WHEN row_number() OVER (ORDER BY Customer_Average_Check DESC) BETWEEN (count(*) OVER () * 0.1) AND (count(*) OVER () * 0.35)
               THEN 'Medium'
           ELSE 'Low'
           END AS Customer_Average_Check_Segment
FROM customers_transactions_summ cts
ORDER BY 1;

CREATE OR REPLACE VIEW customers_transactions_freq AS
SELECT agg.customer_id                                                          AS customer_id,
       (((agg.d_max::date - agg.d_min::date)::numeric) / agg.tr_count::numeric) AS Customer_Frequency
FROM (SELECT c.customer_id              AS customer_id,
             MAX(t.transaction_datetime) AS d_max,
             MIN(t.transaction_datetime) AS d_min,
             COUNT(*)                    AS tr_count
      FROM transactions t
               LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
      GROUP BY c.customer_id) AS agg
ORDER BY 2 DESC;

CREATE OR REPLACE VIEW customers_transactions_freq_segment AS
SELECT ctf.customer_id,
       ctf.Customer_Frequency,
       CASE
           WHEN row_number() OVER (ORDER BY Customer_Frequency DESC) <= (count(*) OVER ())::numeric * 0.1 THEN 'Often'
           WHEN row_number() OVER (ORDER BY Customer_Frequency DESC) BETWEEN ((count(*) OVER () * 0.1)::numeric) AND ((count(*) OVER () * 0.35)::numeric)
               THEN 'Occasionally'
           ELSE 'Rarely'
           END AS Customer_Frequency_Segment
FROM customers_transactions_freq ctf
ORDER BY 1;

CREATE OR REPLACE VIEW inter_from_last_transaction AS
SELECT c.customer_id                      AS customer_id,
       ROUND (EXTRACT (EPOCH FROM ((SELECT * FROM date_of_analysis_formation) - (MAX(t.transaction_datetime))))/86400, 5) AS Last_Transaction_inter
FROM transactions t
         LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
GROUP BY c.customer_id;


CREATE OR REPLACE VIEW customers_transactions_churn_segment AS
SELECT lt.customer_id            AS customer_id,
       lt.Last_Transaction_inter AS Customer_Inactive_Period,
       (lt.Last_Transaction_inter /
        ctf.Customer_Frequency)  AS Customer_Churn_Rate,
       CASE
           WHEN (lt.Last_Transaction_inter / ctf.Customer_Frequency) BETWEEN 0 AND 2 THEN 'Low'
           WHEN (lt.Last_Transaction_inter / ctf.Customer_Frequency) BETWEEN 2 AND 5 THEN 'Medium'
           ELSE 'High'
           END                   AS Customer_Churn_Segment
FROM inter_from_last_transaction lt
         JOIN customers_transactions_freq ctf ON ctf.customer_id = lt.customer_id;

CREATE TABLE Segment_number_assignment
(
    Segment                BIGINT NOT NULL,
    Average_check          VARCHAR CHECK (Average_check IN ('Low', 'Medium', 'High')),
    Frequency_of_purchases VARCHAR CHECK (Frequency_of_purchases IN ('Rarely', 'Occasionally', 'Often')),
    Churn_probability      VARCHAR CHECK (Churn_probability IN ('Low', 'Medium', 'High'))
);

INSERT INTO Segment_number_assignment
VALUES (1, 'Low', 'Rarely', 'Low');
INSERT INTO Segment_number_assignment
VALUES (2, 'Low', 'Rarely', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (3, 'Low', 'Rarely', 'High');
INSERT INTO Segment_number_assignment
VALUES (4, 'Low', 'Occasionally', 'Low');
INSERT INTO Segment_number_assignment
VALUES (5, 'Low', 'Occasionally', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (6, 'Low', 'Occasionally', 'High');
INSERT INTO Segment_number_assignment
VALUES (7, 'Low', 'Often', 'Low');
INSERT INTO Segment_number_assignment
VALUES (8, 'Low', 'Often', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (9, 'Low', 'Often', 'High');
INSERT INTO Segment_number_assignment
VALUES (10, 'Medium', 'Rarely', 'Low');
INSERT INTO Segment_number_assignment
VALUES (11, 'Medium', 'Rarely', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (12, 'Medium', 'Rarely', 'High');
INSERT INTO Segment_number_assignment
VALUES (13, 'Medium', 'Occasionally', 'Low');
INSERT INTO Segment_number_assignment
VALUES (14, 'Medium', 'Occasionally', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (15, 'Medium', 'Occasionally', 'High');
INSERT INTO Segment_number_assignment
VALUES (16, 'Medium', 'Often', 'Low');
INSERT INTO Segment_number_assignment
VALUES (17, 'Medium', 'Often', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (18, 'Medium', 'Often', 'High');
INSERT INTO Segment_number_assignment
VALUES (19, 'High', 'Rarely', 'Low');
INSERT INTO Segment_number_assignment
VALUES (20, 'High', 'Rarely', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (21, 'High', 'Rarely', 'High');
INSERT INTO Segment_number_assignment
VALUES (22, 'High', 'Occasionally', 'Low');
INSERT INTO Segment_number_assignment
VALUES (23, 'High', 'Occasionally', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (24, 'High', 'Occasionally', 'High');
INSERT INTO Segment_number_assignment
VALUES (25, 'High', 'Often', 'Low');
INSERT INTO Segment_number_assignment
VALUES (26, 'High', 'Often', 'Medium');
INSERT INTO Segment_number_assignment
VALUES (27, 'High', 'Often', 'High');

CREATE OR REPLACE VIEW num_of_transactions_by_customers AS
SELECT c.customer_id             AS customer_id,
       COUNT(t.transaction_summ) AS sum_of_transactions
FROM transactions t
         LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
GROUP BY c.customer_id;

CREATE OR REPLACE VIEW stores_list AS
SELECT agg.customer_id,
       agg.transaction_store_id,
       agg.transactions_amount_in_store,
       ntc.sum_of_transactions,
       (agg.transactions_amount_in_store::numeric /
        ntc.sum_of_transactions::numeric)::numeric AS proportion_of_transactions,
       last_date.last_transaction_date
FROM (SELECT agg2.customer_id,
             agg2.transaction_store_id AS transaction_store_id,
             COUNT(*)                  AS transactions_amount_in_store
      FROM (SELECT c.customer_id, t.transaction_store_id
            FROM transactions t
                     LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id) AS agg2
      GROUP BY agg2.customer_id, agg2.transaction_store_id) AS agg
         LEFT JOIN num_of_transactions_by_customers ntc ON agg.customer_id = ntc.customer_id
         LEFT JOIN (SELECT DISTINCT c.customer_id,
                                    t.transaction_store_id,
                                    MAX(t.transaction_datetime) AS last_transaction_date
                    FROM transactions t
                             JOIN cards c ON t.customer_card_id = c.customer_card_id
                    GROUP BY c.customer_id, t.transaction_store_id) AS last_date
                   ON agg.customer_id = last_date.customer_id AND
                      agg.transaction_store_id = last_date.transaction_store_id;

CREATE OR REPLACE VIEW agg_for_three_last_stores AS
SELECT t.transaction_store_id,
       c.customer_id,
       t.transaction_datetime,
       rank() OVER (PARTITION BY c.customer_id ORDER BY t.transaction_datetime DESC) AS rank
FROM transactions t
         JOIN cards c ON t.customer_card_id = c.customer_card_id
ORDER BY c.customer_id, t.transaction_datetime DESC;


CREATE OR REPLACE VIEW three_last_stores AS
SELECT DISTINCT (c.customer_id),
                aftls_r1.transaction_store_id AS nearest_date_store_RANK_1,
                aftls_r2.transaction_store_id AS little_further_date_store_RANK_2,
                aftls_r3.transaction_store_id AS further_date_store_RANK_3
FROM cards c
         LEFT JOIN agg_for_three_last_stores aftls_r1 ON c.customer_id = aftls_r1.customer_id AND aftls_r1.rank = 1
         LEFT JOIN agg_for_three_last_stores aftls_r2 ON c.customer_id = aftls_r2.customer_id AND aftls_r2.rank = 2
         LEFT JOIN agg_for_three_last_stores aftls_r3 ON c.customer_id = aftls_r3.customer_id AND aftls_r3.rank = 3;

CREATE OR REPLACE VIEW Customer_Primary_Store_view AS
SELECT tls.customer_id,
       (CASE
            WHEN tls.nearest_date_store_RANK_1 = tls.little_further_date_store_RANK_2 AND
                 tls.nearest_date_store_RANK_1 = tls.further_date_store_RANK_3
                THEN tls.nearest_date_store_RANK_1
            ELSE (SELECT transaction_store_id
                  FROM stores_list st
                  WHERE tls.customer_id = st.customer_id
                    ORDER BY st.proportion_of_transactions DESC, st.last_transaction_date DESC
                    LIMIT 1
           )
           END) AS Customer_Primary_Store
FROM three_last_stores tls;

CREATE MATERIALIZED VIEW mv_customers AS
(
Select ctsws.customer_id                    AS Customer_ID,
       ctsws.Customer_Average_Check         AS Customer_Average_Check,
       ctsws.Customer_Average_Check_Segment AS Customer_Average_Check_Segment,
       ctfs.customer_frequency              AS Customer_Frequency,
       ctfs.Customer_Frequency_Segment      AS Customer_Frequency_Segment,
       ctcs.Customer_Inactive_Period        AS Customer_Inactive_Period,
       ctcs.Customer_Churn_Rate             AS Customer_Churn_Rate,
       ctcs.Customer_Churn_Segment          AS Customer_Churn_Segment,
       sna.segment                          AS Customer_Segment,
       cpsv.Customer_Primary_Store          AS Customer_Primary_Store
FROM customers_transactions_summ_with_segment ctsws
         JOIN customers_transactions_freq_segment ctfs ON ctsws.customer_id = ctfs.customer_id
         JOIN customers_transactions_churn_segment ctcs ON ctsws.customer_id = ctcs.customer_id
         LEFT JOIN Segment_number_assignment sna ON ctsws.Customer_Average_Check_Segment = sna.Average_check AND
                                                    ctfs.Customer_Frequency_Segment = sna.Frequency_of_purchases AND
                                                    ctcs.Customer_Churn_Segment = sna.Churn_probability
         LEFT JOIN Customer_Primary_Store_view cpsv ON ctsws.customer_id = cpsv.customer_id
         ORDER BY ctsws.customer_id
    );

select *
from mv_customers;
