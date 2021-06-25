import java.sql.*;
import java.util.*;

class Q4 {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
		Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
			Scanner sc = new Scanner(System.in);
			System.out.print("Enter the skill: ");
			String skill = sc.nextLine();
			PreparedStatement stmt = con.prepareStatement("SELECT name, ph_no, address, pjobloc FROM migrantw WHERE skill LIKE ?");
			stmt.setString(1, "\'%" + skill + "%\'");
			ResultSet rSet = stmt.executeQuery();
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
		}
		finally {
			con.close();
		}
	}
}
