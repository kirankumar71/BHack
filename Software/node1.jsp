<html>
<title>
  Reg Page
</title>
<body>
<h1 align="center"><b>Value IR</b></h1>
<%@ page import ="java.sql.*" %>
<%@ page import ="javax.sql.*" %>
<%
String flow =request.getParameter("flow");
String tur =request.getParameter("tur");
Class.forName("com.mysql.jdbc.Driver");

java.sql.Connection con = DriverManager.getConnection("jdbc:mysql://localhost:3306/bhack","root","");

Statement st= con.createStatement();
ResultSet rs;
int i=st.executeUpdate("insert into node1 (tur,flow) values ('"+tur+"','"+flow+"')");
%>
<button name="start" type="button" onclick="location.href='log.jsp'">Refresh</button>
<style>
  canvas{
    border:1px solid black;
  position: center;
    top: 50%;
    left: 0%;
    margin-left: -320px;
    margin-top: -320px;
  }
  button{
    position:center; 
    margin-left:45%;   
    width:150px;
    bottom:280px;
  }
</style>

</body>
</html>