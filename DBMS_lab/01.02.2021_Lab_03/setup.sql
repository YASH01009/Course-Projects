-- i
CREATE TABLE account (
  customer INT PRIMARY KEY,
  balance INT NOT NULL
);

INSERT ALL
INTO account (customer, balance) VALUES (1, 600)
INTO account (customer, balance) VALUES (2, 400)
INTO account (customer, balance) VALUES (3, 400)
INTO account (customer, balance) VALUES (4, 200)
SELECT * FROM dual;

-- query to consider later ranks
SELECT a.customer, COUNT(b.customer) AS rank FROM account a, account b WHERE b.balance >= a.balance GROUP BY a.customer;

-- query to consider current ranks
SELECT a.customer, (SELECT COUNT(*) FROM account)+1-COUNT(b.customer) AS rank FROM account a, account b WHERE b.balance <= a.balance GROUP BY a.customer;

DROP TABLE account;

-- ii
CREATE TABLE transaction (
  transno INT PRIMARY INT,
  acctno INT NOT NULL,
  tdate DATE NOT NULL,
  amount INT NOT NULL
);

INSERT ALL
INTO transaction (transno, acctno, tdate, amount) VALUES (1, 23, TO_DATE('02/01/2021', 'DD/MM/YYYY'), 500)
INTO transaction (transno, acctno, tdate, amount) VALUES (2, 56, TO_DATE('04/01/2021', 'DD/MM/YYYY'), 200)
INTO transaction (transno, acctno, tdate, amount) VALUES (3, 78, TO_DATE('06/01/2021', 'DD/MM/YYYY'), -300)
INTO transaction (transno, acctno, tdate, amount) VALUES (4, 23, TO_DATE('08/01/2021', 'DD/MM/YYYY'), 400)
INTO transaction (transno, acctno, tdate, amount) VALUES (5, 56, TO_DATE('10/01/2021', 'DD/MM/YYYY'), -250)
INTO transaction (transno, acctno, tdate, amount) VALUES (6, 78, TO_DATE('02/01/2021', 'DD/MM/YYYY'), 800)
SELECT * FROM dual;



DROP TABLE transaction;