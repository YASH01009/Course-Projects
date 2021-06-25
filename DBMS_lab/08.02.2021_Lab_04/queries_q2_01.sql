-- i
CREATE TABLE salary_hike (
	eno INT,
	dno INT,
	ename VARCHAR(32) NOT NULL,
	idate DATE,
	amount INT NOT NULL,
	PRIMARY KEY (eno, idate),
	FOREIGN KEY (eno) REFERENCES emp (eno) 
);

-- trigger to validate the increments
CREATE OR REPLACE TRIGGER validate_hikes
BEFORE UPDATE of sal ON emp
REFERENCING NEW AS n OLD AS o
FOR EACH ROW
DECLARE 
	tot_sum INT;
	s_hikes INT;
BEGIN 
	-- get the total budget for that department INTO the variable tot_sum
	SELECT cost INTO tot_sum FROM manpower_budget WHERE dno = :n.deptno;
	-- get the amount that has been used for the salry increments
	SELECT NVL(SUM(amount),0) INTO s_hikes FROM salary_hike WHERE dno = :n.deptno;
	-- if salary icrement is approved for the employee
	IF (s_hikes + :n.sal - :o.sal <= tot_sum) THEN
		dbms_output.put_line('Salary incremented for person with eno '||:n.eno);
		INSERT INTO salary_hike VALUES (:n.eno, :n.deptno, :n.ename, SYSDATE, :n.sal - :o.sal);
	-- if salary increment is disapproved for the employee
	ELSE
		:n.sal := :o.sal;
		dbms_output.put_line('Insufficient Funds for '||:n.eno||' from '||:n.deptno);
	END IF;
END; 
/

UPDATE emp SET sal = sal*4;
SELECT * FROM salary_hike;

-- dropping the table and trigger
DROP TRIGGER validate_hikes;
DROP TABLE salary_hike;
