-- creating tables

CREATE TABLE dept (
    dno INT,
    dname VARCHAR(32) NOT NULL,
    location VARCHAR(32) NOT NULL,
    dmgrno INT NOT NULL,
    PRIMARY KEY (dno)
    -- FOREIGN KEY (dmgrno) REFERENCES emp (eno)
);

CREATE TABLE emp (
    eno INT,
    ename VARCHAR(32),
    sal INT NOT NULL,
    comm INT,
    deptno INT NOT NULL,
    job_type CHAR(2),
    joining_date date,
    PRIMARY KEY (eno),
    FOREIGN KEY (deptno) REFERENCES dept (dno) ON DELETE CASCADE
);

CREATE TABLE manpower_budget (
    dno INT,
    year INT,
    cost INT NOT NULL,
    PRIMARY KEY (dno, year),
    FOREIGN KEY (dno) REFERENCES dept (dno) ON DELETE CASCADE
);

-- inserting tuples

INSERT INTO dept VALUES (1, 'Project Management', 'Guntur', 2);
INSERT INTO dept VALUES (2, 'Marketing', 'Delhi', 9);

INSERT INTO emp VALUES (1, 'Andrew Pavlo', 200, 100, 1, 'SE', date '2020-01-01');
INSERT INTO emp VALUES (2, 'James Musk', 200, 200, 1,'PE', date '2020-12-5');
INSERT INTO emp VALUES (3, 'Richard Jackson', 300, 100, 1, 'RS', date '2020-3-10');
INSERT INTO emp VALUES (4, 'Jack Lite', 100, 200, 1, 'DE', date '2020-9-9');
INSERT INTO emp VALUES (5, 'Mike Tyson', 200, 200, 1, 'HR', date '2020-12-25');
INSERT INTO emp VALUES (6, 'Davis Warner', 300, 100, 2, 'RS', date '2020-2-28');
INSERT INTO emp VALUES (7, 'Joe Biden', 200, 400, 2, 'PE', date '2020-10-20');
INSERT INTO emp VALUES (8, 'Brad Ericcson', 300, NULL, 2, 'DE', date '2020-4-18');
INSERT INTO emp VALUES (9, 'Ben Gwalior', 200, 500, 2, 'SE', date '2020-11-7');

INSERT INTO manpower_budget VALUES (1, 2020, 3200);
INSERT INTO manpower_budget VALUES (2, 2020, 2200);
