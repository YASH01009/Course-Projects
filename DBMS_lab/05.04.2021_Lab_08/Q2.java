import java.sql.*;
import java.io.*;
import java.util.*;

class Q2 {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
        Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
			Scanner sc = new Scanner(System.in);
            System.out.print("Enter the skill: ");
            String skill = sc.nextLine();
            String query = "SELECT name, LPAD(ph_no, 13, '+91') AS ph_no, address, pjobloc FROM migrantw WHERE skill LIKE ?";
            PreparedStatement pstmt = con.prepareStatement(query, ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_UPDATABLE);
            pstmt.setString(1, "%" + skill + "%");
            // display the name, ph no padded with '+91', address and job locations of all migrants
            ResultSet rs = pstmt.executeQuery(); 
            while(rs.next()) {
            	String name = rs.getString("name");
            	String ph_no = rs.getString("ph_no");
            	String address = rs.getString("address");
            	String pjobloc = rs.getString("pjobloc");
            	System.out.println("Name: " + name + ", Contact: " + ph_no + ", Address: " + address + ", Job location: " + pjobloc);
        	}
		} finally {
            con.close();
        }
	}
}