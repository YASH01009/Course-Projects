import java.sql.*;

class Q8 {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
        Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
            String query = "UPDATE migrantw SET ph_no = '912342342345' WHERE enroll_id = 1";
            con.createStatement().executeUpdate(query); 
		} catch(SQLException ex) {
            System.out.println("Error Code: " + ex.getErrorCode());
            System.out.println("Message: " + ex.getMessage());
        } finally {
            con.close();
        }
	}
}