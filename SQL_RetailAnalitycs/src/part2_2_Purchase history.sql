SET search_path = "schema_s21";
CREATE MATERIALIZED VIEW mv_purchase_history AS
SELECT pi.customer_id        AS Customer_ID,
       t.transaction_id      AS Transaction_ID,
       t.transaction_datetime AS Transaction_DateTime,
       pg.group_id AS Group_ID,
SUM (s.sku_purchase_price * ch.sku_amount) AS Group_Cost,
SUM (ch.sku_summ) AS Group_Summ,
SUM (ch.sku_summ_paid) AS Group_Summ_Paid
FROM transactions t
         LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
         JOIN personal_information pi ON pi.customer_id = c.customer_id
         RIGHT JOIN checks ch on t.transaction_id = ch.transaction_id
LEFT JOIN product_grid pg ON ch.sku_id = pg.sku_id
LEFT JOIN stores s ON s.transaction_store_id = t.transaction_store_id AND s.sku_id = ch.sku_id
GROUP BY 1,2,3,4
ORDER BY 1,2, 4;


SELECT * FROM mv_purchase_history;