CREATE TABLE my_user (
	username VARCHAR(32) PRIMARY KEY,
	password VARCHAR(32) NOT NULL,
	dob DATE
);

INSERT INTO my_user VALUES ('xxxx OR x=x', 'agdjasghdg', TO_DATE('01.02.2001', 'DD.MM.YYYY'));
INSERT INTO my_user VALUES ('xxxx', 'pwd123 OR x=x', TO_DATE('01.03.2001', 'DD.MM.YYYY'));
INSERT INTO my_user VALUES ('. . - - AND password=. . . .', 'pwd123 OR x=x', TO_DATE('01.03.2001', 'DD.MM.YYYY'));
INSERT INTO my_user VALUES ('    - - drop table table_name', 'pwd123 OR x=x', TO_DATE('01.03.2001', 'DD.MM.YYYY'));
INSERT INTO my_user VALUES ('xxxxxxxx', 'pwd123 OR x=x', TO_DATE('01.03.2001', 'DD.MM.YYYY'));

SELECT * FROM my_user WHERE username = 'xxxx OR x=x';
SELECT * FROM my_user WHERE username = 'xxxx' AND password = 'pwd123 OR x=x';
SELECT * FROM my_user WHERE username = '. . - - AND password=. . . .';
SELECT * FROM my_user WHERE username = '    - - drop table table_name';

DROP TABLE my_user;
