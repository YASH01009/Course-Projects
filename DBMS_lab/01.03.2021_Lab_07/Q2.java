import java.sql.*;
import java.util.*;

class Q2 {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
		Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
			// query 1
			Scanner sc = new Scanner(System.in);
			System.out.print("Enter the course Id: ");
			int cid = sc.nextInt();
			System.out.print("Enter the percentage required: ");
			int pct = sc.nextInt();
			Statement stmt = con.createStatement();
			ResultSet rSet = stmt.executeQuery("SELECT * FROM ((SELECT * FROM student) NATURAL JOIN (SELECT sroll, attended, total_class FROM attendance WHERE course_id = " + cid + " AND (attended * 100)/total_class >= " + pct + "))");
			ResultSetMetaData rsmd = rSet.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			while (rSet.next()) {
    			for (int i = 1; i <= columnsNumber; i++) {
        			if (i > 1) System.out.print(",  ");
        			String columnValue = rSet.getString(i);
        			System.out.print(rsmd.getColumnName(i) + ": " + columnValue);
        		}
        		System.out.println();
    		}
    		// query 2
    		System.out.print("Enter the name of the instructor: ");
    		sc.nextLine();
    		String instructor = sc.nextLine();
    		stmt = con.createStatement();
    		rSet = stmt.executeQuery("SELECT * FROM ((SELECT sroll, name FROM student) NATURAL JOIN (SELECT sroll, attended, total_class FROM attendance WHERE course_id IN (SELECT cid FROM course WHERE instructor = \'"+ instructor + "\')))");
    		rsmd = rSet.getMetaData();
    		columnsNumber = rsmd.getColumnCount();
    		while(rSet.next()) {
    			for (int i = 1; i <= columnsNumber; i++) {
        			if (i > 1) System.out.print(",  ");
        			String columnValue = rSet.getString(i);
        			System.out.print(rsmd.getColumnName(i) + ": " + columnValue);
        		}
        		System.out.println();
    		}
		}
		finally {
			con.close();
		}
	}
}
