SET search_path = "schema_s21";
CREATE OR REPLACE FUNCTION part5(first_date timestamp, last_date timestamp, added_number_of_transactions int DEFAULT 1,
                                 max_churn_index numeric DEFAULT 0.1, max_discount_share numeric DEFAULT 0.1, allowable_margin numeric DEFAULT 0.1)
    RETURNS TABLE
            (
                "Customer_ID"                 bigint,
                "Start_Date"                  timestamp,
                "End_Date"                    timestamp,
                "Required_Transactions_Count" int,
                "Group_Name"                  varchar,
                "Offer_Discount_Depth"        numeric
            )
AS
$$



SELECT DISTINCT c.customer_id                AS Customer_ID,
                first_date                   AS Start_Date,
                last_date                    AS End_Date,
                ROUND((last_date::DATE - first_date::DATE):: NUMERIC / c.customer_frequency) +
                added_number_of_transactions AS Required_Transactions_Count,
                (SELECT sg.group_name
                 FROM mv_groups g
                          JOIN sku_group sg on g.group_id = sg.group_id
                 WHERE g.customer_id = c.customer_id
                   AND g.group_churn_rate <= max_churn_index
                   AND g.group_discount_share < max_discount_share
                 ORDER BY g.group_affinity_index
                                                LIMIT 1)                    AS Group_Name,
                t.offer_discount_depth * 100 AS Offer_Discount_Depth
FROM mv_customers c
    JOIN fnc_get_group(max_churn_index, max_discount_share, allowable_margin) AS t
ON c.customer_id = t.customer_id
ORDER BY c.customer_id

    $$
    LANGUAGE SQL;

-- Test queries
SELECT *
FROM part5('2020-01-10 10:07:00', '2022-06-28 12:12:12', 3, 0.3, 90, 0.9);

SELECT *
FROM part5('2018-01-16 10:07:00', '2020-08-28 12:12:12', 16, 0.5, 5, 70);
