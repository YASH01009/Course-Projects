import java.sql.*;
import java.util.*;
import java.io.*;
import oracle.jdbc.OracleTypes;

class Q3 {
    public static void findManagerWithEmpId(Connection con, int eid) throws Exception {
        String call = "{call getManagerForEmpid(?,?,?,?)}";
        CallableStatement cs = con.prepareCall(call);
        cs.setInt(1, eid);
        cs.registerOutParameter(2, Types.VARCHAR);
        cs.registerOutParameter(3, Types.VARCHAR);
        cs.registerOutParameter(4, Types.VARCHAR);
        cs.execute();
        String mfname = cs.getString(2);
        String mlname = cs.getString(3);
        String dpt = cs.getString(4);
        System.out.println("Manager details:");
        System.out.println("Name: " + mfname + " " + mlname);
        System.out.println("Department: " + dpt);
    }
    public static void displayEmployeesHiredAfter(Connection con, String jdate, int dpt_no) throws Exception {
        String call = "{call displayEmpAfterDate(?,?,?)}";
        CallableStatement cs1 = con.prepareCall(call);
        cs1.setString(1, jdate);
        cs1.setInt(2, dpt_no);
        cs1.registerOutParameter(3, OracleTypes.CURSOR);
        cs1.execute();
        ResultSet rs = (ResultSet)cs1.getObject(3);
        while(rs.next()) {
            String fname = rs.getString("first_name");
            String lname = rs.getString("last_name");
            String job_title = rs.getString("job_title");
            System.out.println("Name: " + fname + " " + lname + ", Job Title: " + job_title);
        }
    }
    public static void updateEmployee(Connection con, int jid, int eid) throws Exception {
        String query = "UPDATE employees SET salary = salary * 1.05, job_id = ? WHERE employee_id = ?";
        PreparedStatement pstmt = con.prepareStatement(query);
        pstmt.setInt(1, jid);
        pstmt.setInt(2, eid);
        pstmt.executeUpdate();
    }
    public static void departmentsSalary(Connection con) throws Exception {
        Statement stmt = con.createStatement();
        ResultSet rSet = stmt.executeQuery("SELECT department_id, department_name, SUM(salary) AS tot_salary FROM ((SELECT department_id, salary FROM employees) NATURAL JOIN (SELECT department_id, department_name FROM departments)) GROUP BY department_id, department_name HAVING SUM(salary) >= 200");
        FileWriter myFile = new FileWriter("dept_list.txt");
        while(rSet.next()) 
            myFile.write("Departments's Name: " + rSet.getString("department_name") + ", Department's Id: " + rSet.getString("department_id") + ", Total Salary: " + rSet.getString("tot_salary") + "\n");
        myFile.close();
    }
	public static void main(String[] args) throws Exception {
		Class.forName("oracle.jdbc.OracleDriver");
        Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "yashwin");
		try {
			// query 1 (display the manager details of a given employee)
			Scanner sc = new Scanner(System.in);
            System.out.print("Enter the employee id: ");
            int eid = sc.nextInt();
            findManagerWithEmpId(con, eid);
            System.out.print("Enter the department_id and the hire_date (YYYY-MM-DD): ");
            int dpt_no = sc.nextInt();
            String jdate = sc.nextLine().trim();
            System.out.println(dpt_no);
            System.out.println(jdate);
            displayEmployeesHiredAfter(con, jdate, dpt_no);
            System.out.print("Enter the job_id and the employee_id: ");
            int jid = sc.nextInt();
            eid = sc.nextInt();
            updateEmployee(con, jid, eid);
            departmentsSalary(con);
		} finally {
            con.close();
        }
	}
}
