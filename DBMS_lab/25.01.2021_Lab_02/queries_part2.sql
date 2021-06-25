-- i
SELECT name, course_number, semester, year, section_identifier, grade FROM student NATURAL JOIN grade_report NATURAL JOIN section;

-- ii
SELECT name FROM student WHERE student_number IN (SELECT student_number FROM grade_report WHERE grade = 'A') AND student_number NOT IN (SELECT student_number FROM grade_report WHERE grade <> 'A');

-- iii
SELECT course_number, COUNT(*) FROM section NATURAL JOIN grade_report GROUP BY course_number;

-- iv
SELECT name FROM student WHERE student_number IN (SELECT student_number FROM (SELECT student_number, COUNT(*) AS points FROM (SELECT DISTINCT student_number, course_number FROM ((SELECT section_identifier, course_number FROM section WHERE instructor = 'Anderson') NATURAL JOIN (SELECT student_number, section_identifier FROM grade_report))) GROUP BY student_number) WHERE points = (SELECT COUNT(*) FROM (SELECT DISTINCT course_number FROM section WHERE instructor = 'Anderson')));

-- v : year given as input
SELECT student_number, SUM(credit_hours) FROM ((SELECT section_identifier, credit_hours FROM ((SELECT section_identifier, course_number FROM section WHERE year = 7) NATURAL JOIN course)) NATURAL JOIN grade_report) GROUP BY student_number; 

-- vi : students registered for atleast 1 MATH course and exactly 2 CS courses
SELECT name FROM student WHERE student_number IN (SELECT student_number FROM
(SELECT * FROM ((SELECT student_number, cspoints  FROM (SELECT student_number, department, COUNT(*) AS cspoints FROM (SELECT student_number, department FROM ((SELECT student_number, course_number FROM ((SELECT student_number, section_identifier FROM grade_report) NATURAL JOIN (SELECT section_identifier, course_number FROM section))) NATURAL JOIN (SELECT course_number, department FROM course))) GROUP BY student_number, department) WHERE department = 'CS') NATURAL JOIN
(SELECT student_number, mathpoints  FROM (SELECT student_number, department, COUNT(*) AS mathpoints FROM (SELECT student_number, department FROM ((SELECT student_number, course_number FROM ((SELECT student_number, section_identifier FROM grade_report) NATURAL JOIN (SELECT section_identifier, course_number FROM section))) NATURAL JOIN (SELECT course_number, department FROM course))) GROUP BY student_number, department) WHERE department = 'MATH')))
WHERE mathpoints >= 1 AND cspoints = 2);

-- vi : students registered for atleast 1 MATH course and atleast 2 CS courses
SELECT name FROM student WHERE student_number IN (SELECT student_number FROM
(SELECT * FROM ((SELECT student_number, cspoints  FROM (SELECT student_number, department, COUNT(*) AS cspoints FROM (SELECT student_number, department FROM ((SELECT student_number, course_number FROM ((SELECT student_number, section_identifier FROM grade_report) NATURAL JOIN (SELECT section_identifier, course_number FROM section))) NATURAL JOIN (SELECT course_number, department FROM course))) GROUP BY student_number, department) WHERE department = 'CS') NATURAL JOIN
(SELECT student_number, mathpoints  FROM (SELECT student_number, department, COUNT(*) AS mathpoints FROM (SELECT student_number, department FROM ((SELECT student_number, course_number FROM ((SELECT student_number, section_identifier FROM grade_report) NATURAL JOIN (SELECT section_identifier, course_number FROM section))) NATURAL JOIN (SELECT course_number, department FROM course))) GROUP BY student_number, department) WHERE department = 'MATH')))
WHERE mathpoints >= 1 AND cspoints >= 2);

-- vii
DELETE FROM course WHERE course_number = 'MATH2410';

-- viii
SELECT name FROM student WHERE student_number IN (SELECT student_number FROM (SELECT student_number, COUNT(*) AS fails FROM ((SELECT * FROM grade_report WHERE grade = 'F') NATURAL JOIN (SELECT section_identifier FROM section WHERE semester = 'Fall' AND year = 7)) GROUP BY student_number) WHERE fails > 1);

-- xi
UPDATE grade_report SET grade = CASE WHEN grade = 'A' THEN '10' WHEN grade = 'B' THEN '9' WHEN grade = 'C' THEN '8' WHEN grade = 'D' THEN '7' WHEN grade = 'P' then '6' WHEN grade = 'F' THEN '5' ELSE '5' END;
