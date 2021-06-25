import java.sql.*;
import java.io.*;
import java.util.*;

class Q1 {
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
        Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
			Scanner sc = new Scanner(System.in);
            System.out.print("Enter the skill: ");
            String skill = sc.nextLine();
            String query = "SELECT name, ph_no, address, pjobloc FROM migrantw WHERE skill LIKE ?";
            PreparedStatement pstmt = con.prepareStatement(query, ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_UPDATABLE);
            pstmt.setString(1, "%" + skill + "%");
            // display the name, ph no, address and job locations of all migrants
            ResultSet rs = pstmt.executeQuery(); 
            while(rs.next()) {
            	String name = rs.getString("name");
            	String ph_no = rs.getString("ph_no");
            	String address = rs.getString("address");
            	String pjobloc = rs.getString("pjobloc");
            	System.out.println("Name: " + name + ", Contact: " + ph_no + ", Address: " + address + ", Job location: " + pjobloc);
        	}
        	System.out.println();
        	// list third and fourth and first row of the result set in order
        	if(rs.absolute(3) == true) {
        		String name = rs.getString("name");
            	String ph_no = rs.getString("ph_no");
            	String address = rs.getString("address");
            	String pjobloc = rs.getString("pjobloc");
            	System.out.println("Name: " + name + ", Contact: " + ph_no + ", Address: " + address + ", Job location: " + pjobloc);
        	}
        	if(rs.absolute(4) == true) {
        		String name = rs.getString("name");
            	String ph_no = rs.getString("ph_no");
            	String address = rs.getString("address");
            	String pjobloc = rs.getString("pjobloc");
            	System.out.println("Name: " + name + ", Contact: " + ph_no + ", Address: " + address + ", Job location: " + pjobloc);
        	}
        	if(rs.absolute(1) == true) {
        		String name = rs.getString("name");
            	String ph_no = rs.getString("ph_no");
            	String address = rs.getString("address");
            	String pjobloc = rs.getString("pjobloc");
            	System.out.println("Name: " + name + ", Contact: " + ph_no + ", Address: " + address + ", Job location: " + pjobloc);
        	}
        	System.out.println();
        	// update the phone number field in the resultset for a row and display that row
        	System.out.print("Enter the row number you want to update the contact for: ");
        	int row = sc.nextInt();
        	sc.nextLine(); // for the new line character
        	System.out.print("Enter the new contact: ");
        	String contact = sc.nextLine();
        	if(rs.absolute(row) == true) {
        		rs.updateString("ph_no", contact);
        		rs.updateRow();
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