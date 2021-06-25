-- dropping all procedures
DROP PROCEDURE displayEmpAfterDate;
DROP PROCEDURE getManagerForEmpid;
-- dropping all triggers
DROP TRIGGER emp_change;
DROP TRIGGER validate_salary;
-- relaxing constraint to break the loop
ALTER TABLE departments DROP CONSTRAINT dept_mgr_fk;
-- dropping all tables
DROP TABLE employment_change;
DROP TABLE projects;
DROP TABLE employees;
DROP TABLE jobs;
DROP TABLE departments;
