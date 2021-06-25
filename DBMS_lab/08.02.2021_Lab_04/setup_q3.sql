-- creating tables

CREATE TABLE account (
	acctno INT PRIMARY KEY,
	name VARCHAR(32),
	branch VARCHAR(32),
	balance INT
);

CREATE TABLE transaction (
	acctno INT NOT NULL,
	tdate DATE NOT NULL,
	amount INT NOT NULL,
	FOREIGN KEY (acctno) REFERENCES account(acctno)
);

-- inserting tuples

INSERT ALL
INTO account(acctno, name, branch, balance) VALUES (45, 'Virat Kohli', 'Bangaluru', 50)
INTO account(acctno, name, branch, balance) VALUES (47, 'Hardik Pandya', 'Kolkata', 100)
SELECT * FROM dual;
