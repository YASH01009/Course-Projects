-- relaxing constraint to break the loop
ALTER TABLE departments DROP CONSTRAINT dept_mgr_fk;
-- dropping all tables
DROP TABLE projects;
DROP TABLE employees;
DROP TABLE jobs;
DROP TABLE departments;
