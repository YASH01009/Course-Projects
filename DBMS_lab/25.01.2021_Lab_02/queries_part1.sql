-- a
SELECT branch, COUNT(*) FROM (SELECT sroll, branch FROM student NATURAL JOIN offer) GROUP BY branch;

-- b
SELECT sname FROM student WHERE sroll IN (SELECT sroll FROM offer A WHERE osalary >= ALL (SELECT osalary FROM offer));

-- c
SELECT sname, branch FROM student WHERE (sroll, branch) IN (SELECT sroll, branch FROM (SELECT sroll, branch, osalary FROM student NATURAL JOIN offer) WHERE (branch, osalary) IN (SELECT branch, MAX(osalary) FROM student NATURAL JOIN offer GROUP BY branch));

-- d
SELECT branch FROM (SELECT branch, COUNT(*) AS weight FROM (SELECT branch FROM student NATURAL JOIN offer) GROUP BY branch) intertable WHERE intertable.weight = (SELECT MAX(strength) FROM (SELECT branch, COUNT(*) AS strength FROM (SELECT branch FROM student NATURAL JOIN offer) GROUP BY branch));

-- e
SELECT branch, COUNT(*) FROM (SELECT sroll, branch FROM student WHERE sroll NOT IN (SELECT sroll FROM offer)) GROUP BY branch;

-- f
SELECT sname, gender, branch, name, specification, osalary FROM offer NATURAL JOIN student NATURAL JOIN company;

-- g : similar to JEE ranking
SELECT sname FROM student WHERE sroll IN (SELECT sroll FROM (SELECT sroll, COUNT(*) AS points FROM (SELECT * FROM (SELECT sroll, MAX(osalary) AS salary FROM offer GROUP BY sroll) intertable1 INNER JOIN (SELECT MAX(osalary) AS hsalary FROM offer GROUP BY sroll) intertable2 ON intertable2.hsalary >= intertable1.salary) intertable GROUP BY sroll) WHERE points <= (SELECT COUNT(*) FROM (SELECT DISTINCT sroll FROM offer))/2); 
