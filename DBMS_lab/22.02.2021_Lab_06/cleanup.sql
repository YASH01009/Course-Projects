-- dropping all triggers
DROP TRIGGER emp_change;
DROP TRIGGER validate_salary;
DROP TRIGGER decr_salary;
-- dropping all procedures
DROP PROCEDURE incr_salary;
DROP PROCEDURE change_mngr;
-- dropping all functions
DROP FUNCTION departments_manager;
-- relaxing constraint to break the loop
ALTER TABLE departments DROP CONSTRAINT dept_mgr_fk;
-- dropping all tables
DROP TABLE exemployee;
DROP TABLE employment_change;
DROP TABLE projects;
DROP TABLE employees;
DROP TABLE jobs;
DROP TABLE departments;
