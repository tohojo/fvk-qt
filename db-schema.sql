CREATE TABLE 'customers' (id INTEGER PRIMARY KEY, name VARCHAR(255), house VARCHAR(255), balance DOUBLE PRECISION, total_spending DOUBLE PRECISION, card_id VARCHAR(255), credit BOOLEAN, admin BOOLEAN);
CREATE TABLE items (id INTEGER PRIMARY KEY, name VARCHAR(100), price FLOAT);
CREATE TABLE 'transactions' (id INTEGER PRIMARY KEY, customer_id INTEGER, time INTEGER, item_name VARCHAR(255), item_count INTEGER, total_price DOUBLE PRECISION);
