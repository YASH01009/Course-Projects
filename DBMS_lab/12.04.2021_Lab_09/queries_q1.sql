CREATE TABLE my_users (
	username VARCHAR(64),
	password VARCHAR(64) NOT NULL,
	dob DATE,
	PRIMARY KEY (username)
);

INSERT INTO my_users VALUES ('Bairstow', 'Bairstow%123', DATE '1999-12-31');
INSERT INTO my_users VALUES ('Gambhir', 'Gambhir%123', NULL);
INSERT INTO my_users VALUES ('Warner', 'Warner%123', NULL);
INSERT INTO my_users VALUES ('Russell', 'Russell%123', DATE '2002-04-13');
INSERT INTO my_users VALUES ('Bumrah', 'Bumrah%123', NULL);

SELECT * FROM my_users WHERE username = 'xxxx' OR 'x' = 'x';
SELECT * FROM my_users WHERE username = 'xxxx' OR 'x' = 'x' AND password = 'pwd123' OR 'x' = 'x';
SELECT * FROM my_users WHERE username = '..'--' AND password = '....';
SELECT * FROM my_users WHERE username = '..'; DROP TABLE my_users; --' AND password = '....';

DROP TABLE my_users;
