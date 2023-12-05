SET search_path = "schema_s21";
DROP FUNCTION IF EXISTS fnc_average_count(int) CASCADE;
CREATE FUNCTION fnc_average_count(transactions_num int)
    RETURNS table
            (
                customer_id            bigint,
                required_check_measure numeric
            )
AS
$$
BEGIN
    RETURN QUERY
        SELECT DISTINCT q.customer_id,
                        AVG(transaction_summ)
        FROM (SELECT cards.customer_id,
                     transaction_summ,
                     transaction_datetime,
                     ROW_NUMBER() OVER (PARTITION BY cards.customer_id ORDER BY transaction_datetime DESC) AS count
              FROM transactions
                       JOIN cards ON transactions.customer_card_id = cards.customer_card_id
              ORDER BY cards.customer_card_id, transaction_datetime DESC) AS q
        WHERE count <= transactions_num
        GROUP BY q.customer_id;
END;
$$ LANGUAGE plpgsql;


DROP FUNCTION IF EXISTS fnc_average_period(date, date) CASCADE;
CREATE FUNCTION fnc_average_period(first_date date,
                                   last_date date)
    RETURNS table
            (
                customer_id            bigint,
                required_check_measure numeric
            )
AS
$$
BEGIN
    RETURN QUERY
        SELECT DISTINCT p.customer_id,
                        ROUND((SUM(t.transaction_summ) OVER (PARTITION BY p.customer_id)) /
                              (COUNT(t.transaction_id) OVER (PARTITION BY p.customer_id)), 2) avg_check
        FROM cards c
                 JOIN personal_information p ON p.customer_id = c.customer_id
                 JOIN transactions t ON c.customer_card_id = t.customer_card_id
        WHERE transaction_datetime BETWEEN first_date AND last_date
        ORDER BY 1;
END;
$$ LANGUAGE plpgsql;



DROP FUNCTION IF EXISTS fnc_get_group(numeric, numeric, numeric) CASCADE;
CREATE FUNCTION fnc_get_group(max_churn numeric,
                              max_transitions_with_discount numeric,
                              max_margin_share numeric)
    RETURNS table
            (
                customer_id          bigint,
                group_name           varchar(30),
                offer_discount_depth numeric
            )
AS
$$
BEGIN
    RETURN QUERY
        WITH main AS (SELECT g.Customer_ID,
                             g.Group_ID,
                             g.Group_Affinity_Index,
                             (CASE
                                  WHEN ROUND(g.Group_Minimum_Discount / 0.05)::numeric * 0.05
                                      < g.Group_Minimum_Discount
                                      THEN ROUND(g.Group_Minimum_Discount / 0.05)::numeric * 0.05 + 0.05
                                  ELSE ROUND(g.Group_Minimum_Discount / 0.05)::numeric * 0.05 END)  AS round_discount,
                             g.Group_Margin * max_margin_share                                      AS max_margin,
                             g.Group_Minimum_Discount,
                             ROW_NUMBER()
                             OVER (PARTITION BY g.Customer_ID ORDER BY g.Group_Affinity_Index DESC) AS check_priority
                      FROM mv_groups g
                      WHERE g.Group_Churn_Rate <= max_churn
                        AND g.Group_Discount_Share < max_transitions_with_discount
                      ORDER BY 1),
             final_group AS (SELECT DISTINCT ON (m.Customer_ID) m.Customer_ID,
                                                                gs.group_name::varchar(30),
                                                                m.round_discount
                             FROM main m
                                      JOIN sku_group gs ON m.Group_ID = gs.group_id
                             WHERE m.round_discount < m.max_margin
                             ORDER BY m.Customer_ID, m.check_priority)
        SELECT *
        FROM final_group
        ORDER BY 1;

END;
$$ LANGUAGE plpgsql;

DROP FUNCTION IF EXISTS fnc_average_check_growth(int, date, date, int,
                                                 numeric, numeric, numeric, numeric) CASCADE;
CREATE FUNCTION fnc_average_check_growth(calc_method int DEFAULT 1,
                                         first_date date DEFAULT CURRENT_DATE,
                                         last_date date DEFAULT CURRENT_DATE,
                                         transactions_num int DEFAULT 2,
                                         coeff numeric DEFAULT 2,
                                         max_churn numeric DEFAULT 5,
                                         max_transitions_with_discount numeric DEFAULT 3,
                                         max_margin_share numeric DEFAULT 0.5)
    RETURNS table
            (
                customer_id            bigint,
                required_check_measure numeric,
                group_name             varchar,
                offer_discount_depth   numeric
            )
AS
$$
BEGIN
    IF last_date < first_date THEN
        RAISE EXCEPTION '!!!ACHTUNG!!! period ended before it began';
    END IF;
    IF calc_method = 1 THEN
        RETURN QUERY
            SELECT q1.customer_id,
                   round(q1.required_check_measure * coeff, 2),
                   q2.group_name,
                   round(q2.offer_discount_depth::numeric * 100, 2)::numeric as offer_discount_depth
            FROM (SELECT *
                  FROM fnc_average_period(first_date, last_date)) q1
                     JOIN fnc_get_group(max_churn, max_transitions_with_discount, max_margin_share) q2
                          ON q1.customer_id = q2.customer_id;
    ELSIF calc_method = 2 THEN
        RETURN QUERY
            SELECT q11.customer_id,
                   round(q11.required_check_measure * coeff, 2),
                   q22.group_name,
                   round(q22.offer_discount_depth::numeric * 100, 2)::numeric as offer_discount_depth
            FROM (SELECT *
                  FROM fnc_average_count(transactions_num)) q11
                     JOIN fnc_get_group(max_churn, max_transitions_with_discount, max_margin_share) q22
                          ON q11.customer_id = q22.customer_id;
    ELSE
        RAISE EXCEPTION '!!!ACHTUNG!!! select 1 or 2 as calc method parameter';
    END IF;
END;
$$ LANGUAGE plpgsql;



SELECT *
FROM fnc_average_check_growth(1, '2019-01-01'::date, '2025-01-01'::date);

SELECT *
FROM fnc_average_check_growth(1, '2045-01-01'::date, '2025-01-01'::date);

SELECT *
FROM fnc_average_check_growth(2, null, null, 2, 4, 2, 2, 0.1);
