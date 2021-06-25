import java.sql.*;

class Verify {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
		Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
			Statement stmt = con.createStatement();
			ResultSet rSet = stmt.executeQuery("SELECT * FROM student");
			while(rSet.next())
				System.out.println(rSet.getString("sname"));
			System.out.println("Hello!!");
		}
		finally {
			con.close();
		}
	}
}
