CREATE OR REPLACE FUNCTION dist_groups(number_of_groups int, maximum_churn_index numeric,
                                       maximum_consumption_stability_index numeric)
    RETURNS TABLE
            (
                "Customer_ID" bigint,
                "Group_ID"    bigint
            )
AS
$$

SELECT agg.customer_id, agg.group_id
FROM (SELECT g.customer_id,
             g.group_id,
             g.Group_Affinity_Index,
             row_number() over (PARTITION BY g.customer_id ORDER BY g.Group_Affinity_Index DESC) AS number_rows
      FROM (SELECT DISTINCT ON (customer_id, group_id) * FROM mv_groups) AS g
      WHERE g.Group_Churn_Rate <= maximum_churn_index
        AND g.Group_Stability_Index < maximum_consumption_stability_index
      ORDER BY g.Group_Affinity_Index DESC) AS agg
WHERE agg.number_rows <= number_of_groups
ORDER BY 1, 2

$$
    LANGUAGE SQL;


CREATE OR REPLACE FUNCTION part6(number_of_groups int DEFAULT 2, maximum_churn_index numeric DEFAULT 0.1,
                                 maximum_consumption_stability_index numeric DEFAULT 0.1,
                                 maximum_SKU_share_in_percent numeric DEFAULT 5,
                                 allowable_margin_share_in_percent numeric DEFAULT 1)
    RETURNS TABLE
            (
                "Customer_ID"          bigint,
                "SKU_Name"             varchar,
                "Offer_Discount_Depth" numeric
            )
AS
$$
WITH sales AS
         (SELECT t.transaction_id, s.sku_id, pg.group_id
          FROM transactions t
                   JOIN cards c on t.customer_card_id = c.customer_card_id
                   JOIN stores s
                        ON s.transaction_store_id = t.transaction_store_id
                   JOIN product_grid pg on s.sku_id = pg.sku_id),
     group_count AS (SELECT group_id, COUNT(*) AS gr_count
                     FROM sales
                     GROUP BY group_id),
     SKU_count AS (SELECT sales.sku_id, COUNT(*) AS SKU_count
                   FROM sales
                   GROUP BY sku_id),
     SKU_share_in_group AS (SELECT DISTINCT ON (s.sku_id, s.group_id) s.sku_id,
                                                                      s.group_id,
                                                                      sc.SKU_count::NUMERIC / gc.gr_count AS SKU_share
                            FROM sales s
                                     JOIN group_count gc ON gc.group_id = s.group_id
                                     JOIN SKU_count sc ON sc.sku_id = s.sku_id
                            ORDER BY 2, 1),
     pre_pre_disc AS
         (SELECT dg."Customer_ID",
                 dg."Group_ID",
                 tc.sku_id,
                 tc.sku_purchase_price,
                 tc.sku_retail_price,
                 pg.sku_name,
                 (tc.sku_retail_price - tc.sku_purchase_price) AS margin,
                 sg.SKU_share,
                 (CASE
                      WHEN (ROUND((SELECT DISTINCT mg.group_minimum_discount
                                   FROM mv_groups mg
                                   WHERE dg."Customer_ID" = mg.customer_id
                                     AND dg."Group_ID" = mg.group_id) / 0.05) * 0.05)::numeric
                          >= (((allowable_margin_share_in_percent::numeric / 100)::numeric *
                               (tc.sku_retail_price - tc.sku_purchase_price))::numeric /
                              tc.sku_retail_price)::numeric
                          THEN ROUND((SELECT mg.group_minimum_discount
                                      FROM mv_groups mg
                                      WHERE dg."Customer_ID" = mg.customer_id
                                        AND dg."Group_ID" = mg.group_id) / 0.05)::numeric * 5
                      ELSE 0 END)                              AS Offer_Discount_Depth
          FROM dist_groups(number_of_groups, maximum_churn_index, maximum_consumption_stability_index) dg
                   RIGHT JOIN product_grid pg
                              ON dg."Group_ID" = pg.group_id
                   JOIN (SELECT t.transaction_id,
                                t.transaction_summ,
                                t.transaction_store_id,
                                c.customer_id,
                                s.sku_id,
                                s.sku_purchase_price,
                                s.sku_retail_price
                         FROM transactions t
                                  JOIN cards c on t.customer_card_id = c.customer_card_id
                                  JOIN stores s
                                       ON s.transaction_store_id = t.transaction_store_id) AS tc
                        ON dg."Customer_ID" = tc.customer_id AND pg.sku_id = tc.sku_id AND tc.transaction_store_id =
                                                                                           (SELECT cc.customer_primary_store
                                                                                            FROM mv_customers cc
                                                                                            WHERE cc.customer_id = dg."Customer_ID")
                   JOIN SKU_share_in_group sg ON dg."Group_ID" = sg.group_id AND tc.sku_id = sg.sku_id
          WHERE sg.SKU_share < (maximum_SKU_share_in_percent::numeric / 100)
          ORDER BY dg."Customer_ID", dg."Group_ID", margin DESC),
     pre_disc AS
         (SELECT *,
                 row_number()
                 over (PARTITION BY "Customer_ID", "Group_ID" ORDER BY (sku_retail_price - sku_purchase_price) DESC) AS row_num
          FROM pre_pre_disc)
SELECT pre_disc."Customer_ID", pre_disc.sku_name, pre_disc.Offer_Discount_Depth::numeric
FROM pre_disc
WHERE row_num = 1
ORDER BY 1, 2

$$
    LANGUAGE SQL;

-- Test queries
SELECT *
FROM part6(2, 3, 3, 20, 30);

