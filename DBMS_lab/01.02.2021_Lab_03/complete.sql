-- Classwork as submitted on 08.02.2021, issued in last class to this date

-- division using sql
CREATE TABLE works_on (
	essn INT NOT NULL,
	pno INT NOT NULL
);

INSERT ALL
INTO works_on (essn, pno) VALUES (1, 1)
INTO works_on (essn, pno) VALUES (1, 2)
INTO works_on (essn, pno) VALUES (2, 1)
INTO works_on (essn, pno) VALUES (2, 2)
INTO works_on (essn, pno) VALUES (2, 3)
INTO works_on (essn, pno) VALUES (3, 1)
SELECT * FROM dual;

CREATE TABLE s (
	pno INT NOT NULL
);

INSERT ALL
INTO s (pno) VALUES (1)
INTO s (pno) VALUES (2)
SELECT * FROM dual;

-- t = r/s
SELECT essn FROM (SELECT essn, COUNT(*) AS points FROM (SELECT DISTINCT * FROM works_on WHERE pno IN (SELECT * FROM s)) GROUP BY essn) WHERE points = (SELECT COUNT(*) FROM s);

DROP TABLE works_on;
DROP TABLE s;

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
SELECT a.customer, COUNT(b.customer) AS rank 
FROM account a, account b WHERE b.balance >= a.balance 
GROUP BY a.customer;

-- query to consider current ranks
SELECT a.customer, (SELECT COUNT(*) FROM account)+1-COUNT(b.customer) AS rank 
FROM account a, account b WHERE b.balance <= a.balance 
GROUP BY a.customer;

SELECT a.customer, 1+COUNT(b.customer) AS rank
FROM account a LEFT JOIN account b ON  a.balance < b.balance 
GROUP BY a.customer;

SELECT a.customer, 1+(SELECT COUNT(*) FROM account b WHERE b.balance < a.balance) AS rank
FROM account a;

DROP TABLE account;

-- ii
CREATE TABLE transaction (
	transno INT PRIMARY KEY,
	acctno INT NOT NULL,
	tdate DATE NOT NULL,
	amount INT NOT NULL
);

INSERT ALL
INTO transaction(transno, acctno, tdate, amount) VALUES (1, 45, TO_DATE('01/02/2021', 'DD/MM/YYYY'), 500)
INTO transaction(transno, acctno, tdate, amount) VALUES (2, 45, TO_DATE('02/02/2021', 'DD/MM/YYYY'), -200)
INTO transaction(transno, acctno, tdate, amount) VALUES (3, 45, TO_DATE('03/02/2021', 'DD/MM/YYYY'), 300)
INTO transaction(transno, acctno, tdate, amount) VALUES (4, 45, TO_DATE('04/02/2021', 'DD/MM/YYYY'), 800)
INTO transaction(transno, acctno, tdate, amount) VALUES (5, 45, TO_DATE('05/02/2021', 'DD/MM/YYYY'), -450)
INTO transaction(transno, acctno, tdate, amount) VALUES (6, 47, TO_DATE('06/02/2021', 'DD/MM/YYYY'), 200)
INTO transaction(transno, acctno, tdate, amount) VALUES (7, 47, TO_DATE('07/02/2021', 'DD/MM/YYYY'), 300)
INTO transaction(transno, acctno, tdate, amount) VALUES (8, 47, TO_DATE('08/02/2021', 'DD/MM/YYYY'), -100)
INTO transaction(transno, acctno, tdate, amount) VALUES (9, 47, TO_DATE('09/02/2021', 'DD/MM/YYYY'), 200)
INTO transaction(transno, acctno, tdate, amount) VALUES (10, 47, TO_DATE('10/02/2021', 'DD/MM/YYYY'), -500)
SELECT * FROM dual;

-- a
CREATE OR REPLACE VIEW tp AS
SELECT T1.acctno, T1.tdate, T2.amount
FROM transaction T1, transaction T2
WHERE T1.acctno = T2.acctno AND T2.tdate <= T1.tdate;

-- b
CREATE OR REPLACE VIEW cb AS
SELECT acctno, tdate, SUM(amount) AS balance
FROM tp GROUP BY acctno, tdate;

SELECT acctno, MIN(balance)
FROM cb GROUP BY acctno;

DROP VIEW cb;
DROP VIEW tp;
DROP TABLE account;
DROP TABLE transaction;
