SET search_path = "schema_s21";
CREATE MATERIALIZED VIEW Stability AS
SELECT pi.customer_id,
       pg.group_id,
       t.transaction_datetime,
      rank() OVER (PARTITION BY pi.customer_id, pg.group_id ORDER BY t.transaction_datetime ASC) AS R_N
FROM transactions t
         LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
         JOIN personal_information pi ON pi.customer_id = c.customer_id
         RIGHT JOIN checks ch on t.transaction_id = ch.transaction_id
         LEFT JOIN product_grid pg ON ch.sku_id = pg.sku_id
         LEFT JOIN stores s ON s.transaction_store_id = t.transaction_store_id AND s.sku_id = ch.sku_id
GROUP BY 1, 2, 3
ORDER BY 1, 2, 3;

CREATE OR REPLACE VIEW Stability_agg AS
SELECT s1.customer_id,
       s1.group_id,
       s1.transaction_datetime,
       s1.R_N,
       (SELECT s2.transaction_datetime
        FROM Stability s2
        WHERE s1.customer_id = s2.customer_id
          AND s1.group_id = s2.group_id
          AND s2.R_N = (s1.R_N + 1)) AS next_tran_date
FROM Stability s1;

CREATE OR REPLACE VIEW Group_Stability_Index_v AS
SELECT i.customer_id, i.group_id, AVG((i.abs_dev - p.group_frequency) / p.group_frequency) AS Group_Stability_Index
FROM (SELECT sa.customer_id,
             sa.group_id,
             AVG(sa.next_tran_date::DATE - sa.transaction_datetime::DATE) AS abs_dev
      FROM Stability_agg sa
      GROUP BY 1, 2) AS i
         LEFT JOIN mv_Periods p ON i.customer_id = p.customer_id AND i.group_id = p.group_id
GROUP BY 1,2
ORDER BY 1, 2;

CREATE OR REPLACE VIEW Group_params AS
SELECT c.customer_id                               AS Customer_ID,
       pg.group_id                                  AS Group_ID,
      ((SELECT p.Group_Purchase FROM mv_periods p WHERE p.customer_id = c.customer_id AND p.group_id = pg.group_id)::numeric / (SELECT COUNT(DISTINCT a.transaction_id)
                                           FROM mv_purchase_history a
                                           WHERE a.customer_id = c.customer_id
                                             AND a.transaction_datetime BETWEEN (SELECT p1.First_Group_Purchase_Date
                                                                                 FROM mv_Periods p1
                                                                                 WHERE c.customer_id = p1.Customer_ID
                                                                                   AND pg.group_id = p1.Group_ID) AND (SELECT p2.Last_Group_Purchase_Date
                                                                                                                       FROM mv_Periods p2
                                                                                                                       WHERE c.customer_id = p2.Customer_ID
                                                                                                                         AND pg.group_id = p2.Group_ID)
                                           GROUP BY a.customer_id
                                           LIMIT 1)) AS Group_Affinity_Index,
       (SELECT ((SELECT * FROM date_of_analysis_formation)::DATE - p.last_group_purchase_date::DATE)::NUMERIC /
               p.group_frequency
        FROM mv_Periods p
        WHERE c.customer_id = p.Customer_ID
          AND pg.group_id = p.Group_ID)             AS Group_Churn_Rate
FROM transactions t
         LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
         RIGHT JOIN checks ch on t.transaction_id = ch.transaction_id
         LEFT JOIN product_grid pg ON ch.sku_id = pg.sku_id
         LEFT JOIN stores s ON s.transaction_store_id = t.transaction_store_id AND s.sku_id = ch.sku_id
ORDER BY 1, 2;

CREATE OR REPLACE VIEW Group_Margin_v AS
SELECT ph.customer_id,
       ph.group_id,
       (SUM(ph.group_summ_paid) - SUM(ph.group_cost)) AS Group_Margin
FROM mv_purchase_history ph
GROUP BY ph.customer_id, ph.group_id;

CREATE OR REPLACE VIEW Discount_params AS
SELECT td.customer_id,
       td.group_id,
       td.Transaction_with_Discount,
       p.group_purchase,
       td.Transaction_with_Discount::NUMERIC / p.group_purchase AS Group_Discount_Share,
       p.group_min_discount                                     AS Group_Minimum_Discount
FROM (SELECT c.customer_id, pg.group_id, COUNT(*) AS Transaction_with_Discount
      FROM checks ch
               JOIN transactions t ON ch.transaction_id = t.transaction_id
               JOIN cards c on t.customer_card_id = c.customer_card_id
               JOIN product_grid pg ON pg.sku_id = ch.sku_id
      WHERE ch.sku_discount <> 0
      GROUP BY 1, 2) AS td
         LEFT JOIN mv_periods p ON td.customer_id = p.customer_id AND td.group_id = p.group_id;

DROP INDEX IF EXISTS idx_mv_periods, idx_Stability, idx_mv_purchase_history, idx_td_mv_purchase_history;
CREATE INDEX IF NOT EXISTS idx_mv_periods ON mv_periods(Customer_ID, Group_ID);
CREATE INDEX IF NOT EXISTS idx_Stability ON Stability(transaction_datetime);
CREATE INDEX IF NOT EXISTS idx_mv_purchase_history ON mv_purchase_history(Customer_ID, Group_ID, transaction_datetime);
CREATE INDEX IF NOT EXISTS idx_td_mv_purchase_history ON mv_purchase_history(transaction_datetime);
SET enable_seqscan = off;
CREATE MATERIALIZED VIEW mv_Groups AS
SELECT DISTINCT ON (gp.Customer_ID, gp.group_id) gp.Customer_ID                     AS Customer_ID,
                                                 gp.group_id                        AS Group_ID,
                                                 gp.Group_Affinity_Index            AS Group_Affinity_Index,
                                                 gp.Group_Churn_Rate                AS Group_Churn_Rate,
                                                 coalesce(gsi.Group_Stability_Index, 0)          AS Group_Stability_Index,
                                                 gm.Group_Margin                    AS Group_Margin,
                                                 coalesce(dp.Group_Discount_Share, 0)            AS Group_Discount_Share,
                                                 coalesce(dp.Group_Minimum_Discount, 0)          AS Group_Minimum_Discount,
                                                 ph.group_summ_paid / ph.group_summ AS Group_Average_Discount
FROM Group_params gp
         LEFT JOIN Group_Stability_Index_v gsi ON gp.Customer_ID = gsi.customer_id AND gsi.group_id = gp.group_id
         LEFT JOIN mv_purchase_history ph ON ph.customer_id = gp.customer_id AND ph.group_id = gp.group_id
         LEFT JOIN Group_Margin_v gm ON gm.customer_id = gp.customer_id AND gm.group_id = gp.group_id
         LEFT JOIN Discount_params dp ON dp.customer_id = gp.Customer_ID AND dp.group_id = gp.group_id
ORDER BY 1, 2;

Select *
FROM mv_Groups;
