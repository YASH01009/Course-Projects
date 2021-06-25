-- iv
CREATE TABLE employee_history (
	eno INT PRIMARY KEY,
	date_of_joining DATE,
	last_sal_drawn INT,
	date_of_leaving DATE
);

CREATE OR REPLACE TRIGGER history
AFTER INSERT OR DELETE ON emp
REFERENCING NEW AS n OLD AS o
FOR EACH ROW
BEGIN
	-- when an employee joins the organisation
	IF INSERTING THEN 
		INSERT INTO employee_history VALUES (:n.eno, :n.joining_date, NULL, NULL);
	-- when an employee leave the organisation
	ELSE 
		UPDATE employee_history SET last_sal_drawn = :o.sal, date_of_leaving = SYSDATE WHERE eno = :o.eno;
	END IF;
END;
/

SELECT * FROM employee_history;
INSERT INTO emp VALUES (10, 'Donald Trump', 200, 100, 2, 'SE', TO_DATE('08/02/2020', 'DD/MM/YYYY'));
SELECT * FROM employee_history;
DELETE FROM emp WHERE eno = 10;
SELECT * FROM employee_history;

DROP TRIGGER history;
DROP TABLE employee_history;
