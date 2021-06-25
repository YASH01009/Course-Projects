import java.sql.*;
import java.util.*;
import oracle.jdbc.OracleTypes;

class Q1 {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
		Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
			// query 1 (display units which collected more amount than a given amount on a given day)
			Scanner sc = new Scanner(System.in);
            System.out.print("Enter the amount and date (YYYY-MM-DD): ");
            int amount = sc.nextInt();
            String idate = sc.nextLine().trim();
            System.out.println(idate);
            Statement stmt = con.createStatement();
            ResultSet rSet = stmt.executeQuery("SELECT unit FROM doctor NATURAL JOIN opd_payments NATURAL JOIN appointment WHERE p_date = TO_DATE('" + idate +  "', 'YYYY-MM-DD') GROUP BY unit HAVING sum(amt) > " + amount);
            while(rSet.next())
                System.out.println("Unit #:" + rSet.getString("unit"));
    		// query 2 (display all the doctors for a given patient)
    		System.out.print("Enter the patient id: ");
    		int pid = sc.nextInt();
            String call = "{call findAppointments(?,?)}";
            CallableStatement cs = con.prepareCall(call);
            cs.setInt(1, pid);
            cs.registerOutParameter(2, OracleTypes.CURSOR);
            cs.execute();
            ResultSet rs = (ResultSet)cs.getObject(2);
            while(rs.next()) {
                String name = rs.getString("dname");
                String date = rs.getString("adate");
                System.out.println("Doctor's Name: " + name + ", Appointment Date: " + date);
            }
		}
		finally {
			con.close();
		}
	}
}
