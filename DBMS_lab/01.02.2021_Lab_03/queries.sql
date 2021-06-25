-- a
SELECT pname, dname, fname, lname, address, bdate FROM ((SELECT pname, dname, mgr_ssn FROM ((SELECT pname, dnum FROM project WHERE plocation = 'Stafford') NATURAL JOIN (SELECT dname, dnumber AS dnum, mgr_ssn FROM department))) NATURAL JOIN (SELECT fname, lname, address, bdate, ssn AS mgr_ssn FROM employee));

-- b
SELECT fname, lname, salary*1.1 AS salary FROM employee WHERE ssn IN (SELECT essn FROM works_on WHERE pno = (SELECT pnumber FROM project WHERE pname = 'ProductX'));

-- c
SELECT fname, lname, pname, dname FROM (((SELECT pname, pnumber FROM project) NATURAL JOIN (SELECT essn, pno AS pnumber FROM works_on)) NATURAL JOIN ((SELECT ssn AS essn, fname, lname, dno FROM employee) NATURAL JOIN (SELECT dname, dnumber AS dno FROM department))) ORDER BY dname, lname, fname;

-- d
SELECT lname, fname, mlname, mfname FROM ((SELECT lname, fname, super_ssn AS mssn FROM employee WHERE ssn IN (SELECT essn FROM works_on WHERE pno = (SELECT pnumber FROM project WHERE pname = 'ProductX') AND hours > 10)) NATURAL JOIN (SELECT lname AS mlname, fname AS mfname, ssn AS mssn FROM employee));

-- e
SELECT essn FROM works_on WHERE (pno, hours) IN (SELECT pno, hours FROM works_on WHERE essn = '123456789');

-- f
SELECT SUM(salary), MAX(salary), MIN(salary), AVG(salary) FROM (SELECT salary FROM employee WHERE dno = (SELECT dnumber FROM department WHERE dname = 'Research'));

-- g
SELECT dno, COUNT(*) FROM employee WHERE salary > 30000 AND dno IN (SELECT dno FROM (SELECT dno, COUNT(*) AS ecount FROM employee GROUP BY dno) WHERE ecount >= 3) GROUP BY dno;

-- h
CREATE VIEW dept_empdetails AS (SELECT dno, COUNT(*) AS ecount, SUM(salary) AS tsalary FROM employee GROUP BY dno);
INSERT ALL
INTO employee (fname, minit, lname, ssn, bdate, address, sex, salary, super_ssn, dno) VALUES ('Harry', 'N', 'Pavlo', '123123123', TO_DATE('12/01/2016', 'DD/MM/YYYY'), '741 Voss, Houston, TX', 'M', 30000, '333445555', 5)
SELECT * FROM dual;
SELECT * FROM dept_empdetails; 

-- i
CREATE VIEW payroll AS SELECT ssn, fname, lname, dno, salary FROM employee;
UPDATE payroll SET salary = salary+1000 WHERE dno = 5;
SELECT * FROM employee;
