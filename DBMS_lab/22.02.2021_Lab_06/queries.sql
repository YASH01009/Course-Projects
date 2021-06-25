SET SERVEROUTPUT ON;

-- 1
SELECT salary FROM employees WHERE employee_id = 130;

DECLARE
	joe_cnt INT;
	new_salary employees.salary%TYPE;
BEGIN
	-- check if any tuples exist with first_name being 'joe'
	SELECT COUNT(*) INTO joe_cnt FROM employees WHERE first_name = 'joe';
	IF joe_cnt > 0 THEN
		-- if yes, update the salary to min of all of them
		SELECT MIN(salary) INTO new_salary FROM employees WHERE first_name = 'joe';
	ELSE 
		-- if no, update the salary to avg of all salaries
		SELECT AVG(salary) INTO new_salary FROM employees;
	END IF;
	UPDATE employees SET salary = new_salary WHERE employee_id = 130;
END;
/

SELECT salary FROM employees WHERE employee_id = 130;

-- 2
DECLARE 
	efirst_name employees.first_name%TYPE;
	elast_name employees.last_name%TYPE;
	-- declare a cursor for the employees 5 and 10
	CURSOR c IS SELECT first_name, last_name FROM employees WHERE employee_id IN (540,980);
BEGIN
	OPEN c;
	LOOP
		FETCH c INTO efirst_name, elast_name;
		EXIT WHEN c%NOTFOUND;
		-- display the names of employees in the cursor
		dbms_output.put_line(efirst_name||' '||elast_name);
	END LOOP;
	CLOSE c;
END;
/

-- 3
CREATE OR REPLACE FUNCTION departments_manager(departments_no IN INT)
RETURN VARCHAR
IS
	ename VARCHAR(64);
	efirst_name employees.first_name%TYPE;
	elast_name employees.last_name%TYPE;
	id employees.employee_id%TYPE;
BEGIN
	-- select the manager_id attribute from the departments table for the given department
	SELECT manager_id INTO id FROM departments WHERE department_id = departments_no;
	-- get the details of the persom from the employees table using the previously selected manager_id attribute
	SELECT first_name, last_name INTO efirst_name, elast_name FROM employees WHERE employee_id = id;
	ename := efirst_name||' '||elast_name;
	RETURN ename;
END;
/

DECLARE
	departments_no departments.department_id%TYPE;
BEGIN
	departments_no := &departments_no;
	dbms_output.put_line(departments_manager(departments_no));
END;
/

-- 4
CREATE OR REPLACE PROCEDURE change_mngr(departments_no IN departments.department_id%TYPE) 
IS 
	new_mngr employees.employee_id%TYPE;
BEGIN
	-- assumption: only one person has the max salary
	-- select the employee_id with the max salary in the given department
	SELECT employee_id INTO new_mngr FROM employees WHERE 
	salary = (SELECT MAX(salary) FROM employees WHERE department_id = departments_no);
	-- set the manager_id field of the departments table to the prviously selected employee_id
	UPDATE departments SET manager_id = new_mngr WHERE department_id = departments_no;
END;
/

DECLARE
	departments_no departments.department_id%TYPE;
BEGIN
	departments_no := &departments_no;
	change_mngr(departments_no);
END;
/

-- 5
CREATE OR REPLACE TRIGGER decr_salary
BEFORE UPDATE OF salary ON employees
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
BEGIN
	-- reject the update if the new salary is less than the old one
	IF :n.salary < :o.salary THEN
		RAISE_APPLICATION_ERROR('-20124', 'Salary is not allowed to be decremented');
	END IF;
END;
/

-- 6
CREATE OR REPLACE TRIGGER validate_salary
BEFORE INSERT OR UPDATE OF salary ON employees
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE
	allowed_min jobs.min_salary%TYPE;
	allowed_max jobs.max_salary%TYPE;
BEGIN
	-- get the limits for the corresponding job role
	SELECT min_salary, max_salary INTO allowed_min, allowed_max FROM jobs WHERE job_id = :n.job_id;
	-- check the if the updated salary lies in the range
	IF :n.salary < allowed_min OR :n.salary > allowed_max THEN
		RAISE_APPLICATION_ERROR('-20124', 'Salary not allowed');
	END IF;
END;
/

-- 7
SELECT first_name, last_name, salary FROM employees WHERE employee_id IN 
(SELECT project_lead FROM projects WHERE start_date < DATE '1990-12-31' AND department_id = 120);

-- 8
DECLARE
	user_date employees.hire_date%TYPE;
	user_dept employees.department_id%TYPE;
	efirst_name employees.first_name%TYPE;
	elast_name employees.last_name%TYPE;
	ejob_id jobs.job_id%TYPE;
	ejob_title jobs.job_title%TYPE;
	emngr_id employees.manager_id%TYPE;
	ehire_date employees.hire_date%TYPE;
	emngr_dept employees.department_id%TYPE;
	CURSOR c IS SELECT first_name, last_name, job_id, hire_date, manager_id FROM employees ;
BEGIN
	user_date := date '1880-12-12';
	user_dept := &user_dept;
	OPEN c;
	LOOP
		-- fetch all details from the cursor
		FETCH c INTO efirst_name, elast_name, ejob_id, ehire_date, emngr_id;
		EXIT WHEN c%NOTFOUND;
		-- if the current emp has been hired after a given date
		IF ehire_date > user_date THEN
			-- get the department in which his manager works
			SELECT department_id INTO emngr_dept FROM employees WHERE employee_id = emngr_id;
			-- if that dept is same as the given dept
			IF emngr_dept = user_dept THEN
				SELECT job_title INTO ejob_title FROM jobs where job_id = ejob_id;
				dbms_output.put_line('Name: '||efirst_name||' '||elast_name||', '||'Job_title: '||ejob_title);
			END IF;
		END IF;
	END LOOP;
	CLOSE c;
END;
/

-- 9
CREATE TABLE employment_change (
	employee_id INT,
	old_job_id INT
	CONSTRAINT emp_old_job_nn NOT NULL,
	new_job_id INT
	CONSTRAINT emp_new_job_nn NOT NULL,
	old_department_id INT
	CONSTRAINT emp_old_dept_nn NOT NULL,
	new_department_id INT
	CONSTRAINT emp_new_dept_nn NOT NULL,
	change_date DATE,
	type_of_change CHAR(1),
	tuser VARCHAR(32)
);

ALTER TABLE employment_change ADD (
	CONSTRAINT emp_change_pk PRIMARY KEY (employee_id, change_date)
);

CREATE OR REPLACE TRIGGER emp_change
BEFORE UPDATE OF job_id, department_id ON employees
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE
	cur_user VARCHAR(32);
	change_type CHAR(1);
BEGIN
	-- get the current user of the database
	cur_user := USER;
	-- determine the type of change, department_change: D, job_change: J, both: B
	IF :n.job_id <> :o.job_id THEN change_type := 'J';
	ELSIF :n.department_id <> :o.department_id THEN change_type := 'D';
	ELSIF :n.department_id <> :o.department_id AND :n.job_id <> :o.job_id THEN change_type := 'B';
	ELSE change_type := 'N';
	END IF;
	-- accordingly modify the employment_change table
	INSERT INTO employment_change VALUES (:n.employee_id, :o.job_id, :n.job_id, :o.department_id, 
		:n.department_id, SYSDATE, change_type, cur_user);
END;
/


CREATE TABLE exemployee (
	employee_id INT,
	first_name VARCHAR(32),
	last_name VARCHAR(32),
	department_id INT,
	release_date DATE
);

ALTER TABLE exemployee ADD (
	CONSTRAINT exemp_pk PRIMARY KEY (employee_id, release_date)
);

CREATE OR REPLACE TRIGGER emp_change
BEFORE DELETE ON employees
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
BEGIN
	INSERT INTO exemployee VALUES (:o.employee_id, :o.first_name, :o.last_name, :o.department_id, SYSDATE);
END;
/

-- 10
CREATE OR REPLACE PROCEDURE incr_salary(departments_no IN departments.department_id%TYPE)
IS
	employee_no employees.employee_id%TYPE;
	-- select all the employee_ids from the given department
	CURSOR c IS SELECT employee_id FROM employees WHERE department_id = departments_no;
BEGIN
	OPEN c;
	LOOP 
		FETCH c INTO employee_no;
		EXIT WHEN c%NOTFOUND;
		-- update salary for each member of the department
		UPDATE employees SET salary = salary+20 WHERE employee_id = employee_no;
	END LOOP;
	CLOSE c;
END;
/

DECLARE
	departments_no departments.department_id%TYPE;
BEGIN
	departments_no := &departments_no;
	incr_salary(departments_no);
END;
/

-- 12
CREATE SEQUENCE tr_ids
START WITH 1
INCREMENT BY 1
NO CACHE
NO CYCLE;

CREATE OR REPLACE TRIGGER can_issue
BEFORE INSERT ON book_issue
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE
	hold_cnt INT;
	can_i_issue CHAR(1);
	tbook_id book_stock.book_id%TYPE;
	CURSOR c IS 
	-- get all the books that the user is currently holding
	SELECT book_id FROM book_issue WHERE card_no = :n.card_no MINUS SELECT book_id FROM book_return WHERE card_no = :n.card_no;
BEGIN
	-- assume we can sanction the issue
	can_i_issue := 'Y';
	-- find how many books the user currently has
	hold_cnt := c%ROWCOUNT;
	-- reject if he has more than equal to 3
	IF hold_cnt >= 3 THEN
		can_i_issue := 'N';
		RAISE_APPLICATION_ERROR('-20124', 'Each user can be allowed for 3 books only');
	END IF;
	-- check if the user already has another copy of the same book
	OPEN c;
	LOOP
		FETCH c INTO tbook_id;
		EXIT WHEN c%NOTFOUND;
		IF :n.book_id = tbook_id THEN
			can_i_issue := 'N';
			RAISE_APPLICATION_ERROR('-20124', 'A user can not get two copies of the same book');
		END IF;
	END LOOP;
	CLOSE c;
	-- finally update no of copies if the request is approved
	IF can_i_issue = 'Y' THEN 
		UPDATE book_stock SET copies = copies-1 WHERE book_id = :n.book_id;
	END IF;
END;
/

CREATE OR REPLACE TRIGGER return_details
BEFORE INSERT ON book_return
REFERENCING NEW AS n OLD AS o
FOR EACH ROW
DECLARE
	tdue_date book_issue.due_date%TYPE;
BEGIN
	-- check for the due date and accordingly impose fines
	SELECT due_date INTO tdue_date FROM book_issue WHERE 
	issue_date = :n.issue_date AND book_id = :n.book_id AND card_no = :n.card_no;
	IF tdue_date < :n.return_date THEN
		INSERT INTO fines VALUES (:n.card_no, 50);
	END IF;
	UPDATE book_stock SET copies = copies+1 WHERE book_id = :n.book_id;
END;
/

SET SERVEROUTPUT OFF;
