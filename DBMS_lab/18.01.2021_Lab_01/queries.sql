/** 1. **/

SELECT sname, branch FROM student WHERE 
sroll IN (SELECT DISTINCT sroll FROM offer);

/** 2. **/

SELECT sname FROM student WHERE 
sroll IN (SELECT sroll FROM offer WHERE osalary > 100000);

/** 3. **/

SELECT * FROM student WHERE 
sroll IN (SELECT sroll FROM interview) AND NOT sroll IN(SELECT sroll FROM offer);

SELECT * FROM student WHERE 
sroll IN (SELECT sroll FROM interview WHERE 
NOT sroll IN (SELECT sroll FROM offer));

/** 4. **/

SELECT sname FROM student WHERE
sroll IN (SELECT sroll FROM offer) AND programme = 'B';

SELECT sname FROM student WHERE
sroll IN (SELECT sroll FROM offer WHERE 
sroll IN (SELECT sroll FROM student WHERE programme = 'B'));

SELECT sname FROM student WHERE
sroll IN (SELECT sroll FROM student WHERE 
programme = 'B' AND sroll IN (SELECT sroll FROM offer));

/** 5. **/

SELECT sname FROM student WHERE
sroll IN (SELECT sroll FROM offer) AND gender = 'F';

SELECT sname FROM student WHERE
sroll IN (SELECT sroll FROM offer WHERE 
sroll IN (SELECT sroll FROM student WHERE gender = 'F'));

SELECT sname FROM student WHERE
sroll IN (SELECT sroll FROM student WHERE 
gender = 'F' AND sroll IN (SELECT sroll FROM offer));
