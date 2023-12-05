SET search_path = "schema_s21";
CREATE MATERIALIZED VIEW mv_Periods AS
SELECT pi.customer_id              AS                       Customer_ID,
       pg.group_id                 AS                       Group_ID,
       MIN(t.transaction_datetime) AS                       First_Group_Purchase_Date,
       MAX(t.transaction_datetime) AS                       Last_Group_Purchase_Date,
       COUNT(t.transaction_id)     AS                       Group_Purchase,
       ((MAX(t.transaction_datetime)::DATE - MIN(t.transaction_datetime)::DATE)::NUMERIC + 1) /
       COUNT(t.transaction_id)     AS                       Group_Frequency,
       MIN(ch.sku_discount::NUMERIC / ch.sku_summ::NUMERIC) AS Group_Min_Discount
FROM transactions t
         LEFT JOIN cards c ON t.customer_card_id = c.customer_card_id
         JOIN personal_information pi ON pi.customer_id = c.customer_id
         RIGHT JOIN checks ch on t.transaction_id = ch.transaction_id
         LEFT JOIN product_grid pg ON ch.sku_id = pg.sku_id
         LEFT JOIN stores s ON s.transaction_store_id = t.transaction_store_id AND s.sku_id = ch.sku_id
GROUP BY pi.customer_id, pg.group_id
ORDER BY 1, 2;

select *
from mv_Periods;