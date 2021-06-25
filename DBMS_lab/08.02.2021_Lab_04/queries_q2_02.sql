-- ii
SELECT a.eno, a.ename FROM emp a WHERE (SELECT COUNT(*) FROM emp b WHERE b.sal+NVL(b.comm,0) > a.sal+NVL(a.comm,0) AND a.deptno = b.deptno) < 3;
