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

SET AUTOTRACE ON;
SELECT * FROM employees WHERE job_id = 511 AND department_id = 120;
SET AUTOTRACE OFF;

-- creating the cluster
CREATE CLUSTER emp_cluster (department_id INT, job_id INT);
-- creating the index on the cluster
CREATE INDEX emp_cluster_idx ON CLUSTER emp_cluster;

-- creating the tables
CREATE TABLE emp_120_511
CLUSTER emp_cluster (department_id, job_id)
AS SELECT * FROM employees WHERE department_id = 120 AND job_id = 511;

CREATE TABLE emp_230_511
CLUSTER emp_cluster (department_id, job_id)
AS SELECT * FROM employees WHERE department_id = 230 AND job_id = 511;

CREATE TABLE emp_120_512
CLUSTER emp_cluster (department_id, job_id)
AS SELECT * FROM employees WHERE department_id = 120 AND job_id = 512;

CREATE TABLE emp_230_512
CLUSTER emp_cluster (department_id, job_id)
AS SELECT * FROM employees WHERE department_id = 230 AND job_id = 512;

SET AUTOTRACE ON;
SELECT * FROM employees WHERE job_id = 511 AND department_id = 120;
SET AUTOTRACE OFF;

DROP INDEX emp_cluster_idx;
DROP CLUSTER emp_cluster INCLUDING TABLES;
DROP TABLE employees;
DROP TABLE jobs;
DROP TABLE departments;

