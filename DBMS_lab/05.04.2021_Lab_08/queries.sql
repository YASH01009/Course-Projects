-- 3, 4
CREATE TABLE company_req (
	cname VARCHAR(32),
	jobtype VARCHAR(32),
	jobloc VARCHAR(32), -- single valued
	vacancy NUMBER NOT NULL,
	skill_req VARCHAR(32) NOT NULL, -- single valued
	CONSTRAINT comp_req_pk PRIMARY KEY (cname, jobtype, jobloc)
);

CREATE TABLE place_migrant (
	cname VARCHAR(32) NOT NULL,
	enroll_id NUMBER,
	jobtype VARCHAR(32) NOT NULL,
	osalary NUMBER NOT NULL,
	jobloc VARCHAR(32) NOT NULL, -- single valued
	place_date DATE NOT NULL,
	CONSTRAINT place_migrant_pk PRIMARY KEY (enroll_id),
	CONSTRAINT place_migrant_id_fk FOREIGN KEY (enroll_id) REFERENCES migrantw
);

CREATE OR REPLACE TRIGGER place_workers
BEFORE INSERT ON company_req
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE
	tenroll_id migrantw.enroll_id%TYPE;
	tpjobloc migrantw.pjobloc%TYPE;
	tskill migrantw.skill%TYPE;
	CURSOR c IS SELECT enroll_id, pjobloc, skill FROM migrantw WHERE status = 'U' AND pjobloc LIKE '%'||:n.jobloc||'%' AND skill LIKE '%'||:n.skill_req||'%' ORDER BY enroll_date;
BEGIN
	OPEN c;
	LOOP 
		FETCH c INTO tenroll_id, tpjobloc, tskill;
		EXIT WHEN c%NOTFOUND;
		IF :n.vacancy > 0 THEN
			-- insert the details into place_migrant relation
			INSERT INTO place_migrant VALUES (:n.cname, tenroll_id, :n.jobtype, 500, :n.jobloc, SYSDATE);
			-- mark the worker placed in migrantw
			UPDATE migrantw SET status = 'P' WHERE enroll_id = tenroll_id;
			:n.vacancy := :n.vacancy - 1;
		END IF;
	END LOOP;
	CLOSE c;
END;
/

INSERT INTO company_req VALUES ('Bajaj', 'Full time', 'Mumbai', 3, 'Electrician');

SELECT * FROM company_req;
SELECT * FROM place_migrant;
SELECT DISTINCT enroll_id, status FROM migrantw;

-- 5
ROLLBACK;

SELECT * FROM company_req;
SELECT * FROM place_migrant;
SELECT DISTINCT enroll_id, status FROM migrantw;

-- 6
INSERT INTO company_req VALUES ('Bajaj', 'Full time', 'Mumbai', 3, 'Electrician');

SELECT * FROM company_req;
SELECT * FROM place_migrant;
SELECT DISTINCT enroll_id, status FROM migrantw;

CREATE OR REPLACE VIEW placed_migrants_view AS SELECT * FROM 
((SELECT DISTINCT name, ph_no, enroll_id FROM migrantw) NATURAL JOIN 
(SELECT cname, osalary, jobloc, place_date, enroll_id FROM place_migrant));
SELECT * FROM placed_migrants_view;

-- 7
SELECT cname, COUNT(*) AS migrants, SUM(osalary) AS salary FROM placed_migrants_view GROUP BY cname;

-- 9
SET AUTOCOMMIT OFF;
INSERT INTO place_migrant VALUES ('Google', 6, 'Part time', 300, 'Mumbai', SYSDATE);
COMMIT;
SELECT * FROM place_migrant;
UPDATE place_migrant SET osalary = 500 WHERE enroll_id = 6;
SELECT * FROM place_migrant;
ROLLBACK;
SELECT * FROM place_migrant;

-- 10
SELECT cname, jobtype, jobloc, vacancy FROM company_req WHERE vacancy > 0; 

-- 12
-- obs: When we run the exit command in SQL*Plus, all the changes are committed even during AUTOCOMMIT OFF;

-- 13
CREATE TABLE departments ( 
    department_id INT,
    department_name VARCHAR2(30) 
    CONSTRAINT dept_name_nn NOT NULL,
    PRIMARY KEY (department_id)
);

CREATE TABLE employees ( 
    employee_id INT,
    first_name VARCHAR2(32),
    last_name VARCHAR2(32) 
    CONSTRAINT emp_last_name_nn NOT NULL,
    hire_date DATE 
    CONSTRAINT emp_hire_date_nn NOT NULL,
    job_id INT 
    CONSTRAINT emp_job_nn NOT NULL,
    salary INT
    CONSTRAINT emp_salary_min CHECK (salary > 0),
    manager_id INT,
    department_id INT
    CONSTRAINT emp_dept_id_fk REFERENCES departments
);

INSERT INTO departments VALUES (120, 'Project Management');
INSERT INTO departments VALUES (230, 'Marketing');

INSERT INTO employees VALUES (130, 'Andrew', 'Pavlo', date '2020-01-01', 511, 400, 210, 120);
INSERT INTO employees VALUES (210, 'James', 'Musk', date '2020-12-5', 512, 300, 980, 120);
INSERT INTO employees VALUES (320, 'Richard', 'Jackson', date '2020-3-10', 511, 750, 980, 120);
INSERT INTO employees VALUES (430, 'Jack', 'Lite', date '2020-9-9', 512, 450, 210, 120);
INSERT INTO employees VALUES (540, 'Mike', 'Tyson', date '2020-12-25', 511, 550, 980, 120);
INSERT INTO employees VALUES (650, 'Davis', 'Warner', date '2020-2-28', 512, 300, 980, 230);
INSERT INTO employees VALUES (760, 'Joe', 'Biden', date '2020-10-20', 511, 450, 210, 230);
INSERT INTO employees VALUES (870, 'Brad', 'Ericcson', date '2020-4-18', 512, 300, 980, 230);
INSERT INTO employees VALUES (980, 'Ben', 'Gwalior', date '2020-11-7', 511, 750, 210, 230);

CREATE OR REPLACE VIEW emp_dept_view AS (SELECT * FROM ((SELECT * FROM employees) NATURAL JOIN (SELECT * FROM departments)));

SELECT * FROM emp_dept_view;

CREATE OR REPLACE TRIGGER emp_dept_trigger
INSTEAD OF INSERT ON emp_dept_view
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
BEGIN
	INSERT INTO employees VALUES (:n.employee_id, :n.first_name, 
		:n.last_name, :n.hire_date, :n.job_id, :n.salary, :n.department_id, :n.manager_id);
END;
/

INSERT INTO emp_dept_view VALUES (230, 223, 'Mahesh', 'Babu', date '2020-09-11', 511, 300, 230, 'Marketing');

SELECT * FROM employees;

DROP TRIGGER emp_dept_trigger;
DROP VIEW emp_dept_view;
DROP TABLE employees;
DROP TABLE departments;


