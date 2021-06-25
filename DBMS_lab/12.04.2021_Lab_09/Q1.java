import java.sql.*;
import java.io.*;
import java.util.*;

class Q1 {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
        Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
            // table creation
            Statement stmt = con.createStatement();
            stmt.executeQuery("CREATE TABLE my_users (" +
                                "username VARCHAR(64)," +
                                "password VARCHAR(64) NOT NULL," +
                                "dob DATE," +
                                "PRIMARY KEY (username)" +
                            ")");
            // insert the tuples
            stmt.executeUpdate("INSERT INTO my_users VALUES ('Bairstow', 'Bairstow%123', DATE '1999-04-12')");
            stmt.executeUpdate("INSERT INTO my_users VALUES ('Gambhir', 'Gambhir%123', NULL)");
            stmt.executeUpdate("INSERT INTO my_users VALUES ('Warner', 'Warner%123', NULL)");
            stmt.executeUpdate("INSERT INTO my_users VALUES ('Russell', 'Russell%123', DATE '2002-12-31')");
            stmt.executeUpdate("INSERT INTO my_users VALUES ('Bumrah', 'Bumrah%123', NULL)");

            // Using create statements
            // query 1
            try {
                System.out.println("Query 1:");
                ResultSet rs = stmt.executeQuery("SELECT * FROM my_users WHERE username = 'xxxx' OR 'x' = 'x'");
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (SQLException ex) {
                System.out.println("Error Code: " + ex.getErrorCode());
                System.out.println("Message: " + ex.getMessage());
            }
            
            // query 2
            try {
                System.out.println("Query 2:");
                ResultSet rs = stmt.executeQuery("SELECT * FROM my_users WHERE username = 'xxxx' OR 'x' = 'x' AND password = 'pwd123' OR 'x' = 'x'");
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (SQLException ex) {
                System.out.println("Error Code: " + ex.getErrorCode());
                System.out.println("Message: " + ex.getMessage());
            }
            
            // query 3
            try {
                System.out.println("Query 3:");
                ResultSet rs = stmt.executeQuery("SELECT * FROM my_users WHERE username = '..'--' AND password = '....'");
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (SQLException ex) {
                System.out.println("Error Code: " + ex.getErrorCode());
                System.out.println("Message: " + ex.getMessage());
            }

            // query 4
            try {
                System.out.println("Query 4:");
                ResultSet rs = stmt.executeQuery("SELECT * FROM my_users WHERE username = '..'; DROP TABLE my_users; --' AND password = '....'");
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (SQLException ex) {
                System.out.println("Error Code: " + ex.getErrorCode());
                System.out.println("Message: " + ex.getMessage());
            }

            // Using prepared statements
            // query 1
            String query = "SELECT * FROM my_users WHERE username = ?";
            PreparedStatement pstmt = con.prepareStatement(query);
            try {
                System.out.println("Query 1:");
                pstmt.setString(1, "xxxx' OR 'x' = 'x");
                ResultSet rs = pstmt.executeQuery();
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (Exception e) {
                e.printStackTrace();
            }

            // query 2
            query = "SELECT * FROM my_users WHERE username = ? AND password = ?";
            try {
                System.out.println("Query 2:");
                pstmt = con.prepareStatement(query);
                pstmt.setString(1, "xxxx' OR 'x' = 'x");
                pstmt.setString(2, "pwd123' OR 'x' = 'x");
                ResultSet rs = pstmt.executeQuery();
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (Exception e) {
                e.printStackTrace();
            }
            
        	// query 3
            try {
                System.out.println("Query 3:");
                pstmt.setString(1, "..'--");
                pstmt.setString(2, "....");
                ResultSet rs = pstmt.executeQuery();
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (Exception e) {
                e.printStackTrace();
            }
        	
        	//query 4
            try {
                System.out.println("Query 4:");
                pstmt.setString(1, "'; DROP TABLE my_users; --");
                pstmt.setString(2, "....");
                ResultSet rs = pstmt.executeQuery();
                while(rs.next())
                    System.out.println(rs.getString("username"));
                System.out.println();
            } catch (Exception e) {
                e.printStackTrace();
            }
		} catch (SQLException ex) {
            System.out.println("Error Code: " + ex.getErrorCode());
            System.out.println("Message: " + ex.getMessage());
        } finally {
            // drop the table
            con.createStatement().executeQuery("DROP TABLE my_users");
            con.close();
        }
	}
}