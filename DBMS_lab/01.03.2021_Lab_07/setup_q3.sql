CREATE TABLE departments ( 
    department_id INT,
    department_name VARCHAR2(30) 
    CONSTRAINT dept_name_nn NOT NULL,
    manager_id INT
);

ALTER TABLE departments ADD ( 
    CONSTRAINT dept_id_pk PRIMARY KEY (department_id)
);

CREATE TABLE jobs ( 
    job_id  INT,
    job_title VARCHAR2(32)
    CONSTRAINT job_title_nn NOT NULL,
    min_salary INT,
    max_salary INT
);

ALTER TABLE jobs ADD ( 
    CONSTRAINT job_id_pk PRIMARY KEY(job_id)
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
);
       
ALTER TABLE employees ADD ( 
    CONSTRAINT emp_emp_id_pk PRIMARY KEY (employee_id),
    CONSTRAINT emp_dept_fk FOREIGN KEY (department_id) REFERENCES departments,
    CONSTRAINT emp_job_fk FOREIGN KEY (job_id) REFERENCES jobs (job_id)
);

INSERT INTO departments VALUES (120, 'Project Management', 210);
INSERT INTO departments VALUES (230, 'Marketing', 980);

INSERT INTO jobs VALUES (511, 'Software Engineer', 200, 800);
INSERT INTO jobs VALUES (512, 'Application Engineer', 250, 900);

INSERT INTO employees VALUES (130, 'Andrew', 'Pavlo', date '2020-01-01', 511, 400, 210, 120);
INSERT INTO employees VALUES (210, 'James', 'Musk', date '2020-12-5', 512, 300, 980, 120);
INSERT INTO employees VALUES (320, 'Richard', 'Jackson', date '2020-3-10', 511, 750, 980, 120);
INSERT INTO employees VALUES (430, 'Jack', 'Lite', date '2020-9-9', 512, 450, 210, 120);
INSERT INTO employees VALUES (540, 'Mike', 'Tyson', date '2020-12-25', 511, 550, 980, 120);
INSERT INTO employees VALUES (650, 'Davis', 'Warner', date '2020-2-28', 512, 300, 980, 230);
INSERT INTO employees VALUES (760, 'Joe', 'Biden', date '2020-10-20', 511, 450, 210, 230);
INSERT INTO employees VALUES (870, 'Brad', 'Ericcson', date '2020-4-18', 512, 300, 980, 230);
INSERT INTO employees VALUES (980, 'Ben', 'Gwalior', date '2020-11-7', 511, 750, 210, 230);

ALTER TABLE employees ADD (
    CONSTRAINT emp_manager_fk FOREIGN KEY (manager_id) REFERENCES employees
);

ALTER TABLE departments ADD ( 
    CONSTRAINT dept_mgr_fk FOREIGN KEY (manager_id) REFERENCES employees (employee_id)
);

CREATE TABLE projects (
    project_id INT,
    project_name VARCHAR(32),
    start_date DATE
    CONSTRAINT p_start_date_nn NOT NULL,
    department_id INT,
    project_lead INT
);

ALTER TABLE projects ADD (
    CONSTRAINT projects_id_pk PRIMARY KEY (project_id),
    CONSTRAINT projects_dept_id FOREIGN KEY (department_id) REFERENCES departments,
    CONSTRAINT projects_lead FOREIGN KEY (project_lead) REFERENCES employees
);

-- inserting tuples

INSERT INTO projects VALUES (221, 'Wireless Network', date '1988-05-01', 120, 320);
INSERT INTO projects VALUES (357, 'Microprocessor', date '1984-04-23', 230, 870);

CREATE OR REPLACE PROCEDURE getManagerForEmpid(eid IN NUMBER, mgr_fname OUT VARCHAR, mgr_lname OUT VARCHAR, dpt OUT VARCHAR)
AS
    mgr_no employees.employee_id%TYPE;
    dpt_id employees.department_id%TYPE;
BEGIN
    SELECT manager_id INTO mgr_no FROM employees WHERE employee_id = eid;
    SELECT first_name, last_name, department_id INTO mgr_fname, mgr_lname, dpt_id FROM employees WHERE employee_id = mgr_no;
    SELECT department_name INTO dpt FROM departments WHERE department_id = dpt_id;
END;
/

CREATE OR REPLACE PROCEDURE displayEmpAfterDate(jdate IN VARCHAR, dpt_no IN NUMBER, cur OUT SYS_REFCURSOR) AS
BEGIN
    OPEN cur FOR SELECT E.first_name, E.last_name, J.job_title FROM employees E NATURAL JOIN jobs J WHERE E.hire_date > TO_DATE(jdate, 'YYYY-MM-DD') AND E.manager_id IN (SELECT employee_id FROM employees WHERE department_id = dpt_no);
END;
/

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
