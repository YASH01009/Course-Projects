CREATE TABLE migrantw (	
	enroll_id NUMBER PRIMARY KEY, 
	name VARCHAR(64) NOT NULL, 
	skill VARCHAR2(64) NOT NULL, 
	address VARCHAR(32) NOT NULL, 
	pjobloc VARCHAR2(64) NOT NULL, 
	ph_no CHAR(10) NOT NULL, 
	enroll_date DATE NOT NULL,
	status CHAR(1) NOT NULL
);

INSERT INTO migrantw VALUES (1, 'Ruchit Jain', 'TV Mechanic, Electrician', 'Bangalore, KA', 'Mumbai, Bangalore, Delhi', '9876543210', DATE '2020-03-13', 'U');
INSERT INTO migrantw VALUES (2, 'Pranab Jaiswal', 'Electrician', 'Mumbai, MH', 'Mumbai, Noida, Hyderabad', '9898656532', DATE '2020-04-12', 'U');
INSERT INTO migrantw VALUES (3, 'Ankit Singh', 'TV Mechanic', 'Channai, TN', 'Mumbai, Noida, Delhi', '9960754382', DATE '2020-03-23', 'U');
INSERT INTO migrantw VALUES (4, 'Nalin Gowd', 'Security Gaurd', 'Mumbai, MH', 'Bangalore, Hyderabad', '9473205723', DATE '2020-01-30', 'U');
INSERT INTO migrantw VALUES (5, 'Prakash Janwar', 'TV Mechanic, Cab Driver, Security Gaurd', 'Lucknow, UP', 'Mumbai, Channai', '8196385035', DATE '2020-05-17', 'U');
INSERT INTO migrantw VALUES (6, 'Pramod Lolak', 'TV Mechanic, Carpenter', 'Mangalore, KA', 'Mumbai, Delhi, Channai', '9539667282', DATE '2020-06-21', 'U');
