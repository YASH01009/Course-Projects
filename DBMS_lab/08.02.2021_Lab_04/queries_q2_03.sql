-- iii
CREATE OR REPLACE VIEW dept_location AS
SELECT dno, location FROM dept;

SELECT * FROM dept;
UPDATE dept_location SET location = 'Hyderabad' WHERE dno = 1;
SELECT * FROM dept;

DROP VIEW dept_location;
