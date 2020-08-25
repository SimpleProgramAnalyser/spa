<?xml version="1.0" encoding="utf-16"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml" xmlns:set="http://exslt.org/sets" extension-element-prefixes="set">
	<xsl:param name="comment">comment</xsl:param>
	<xsl:template match="test_results">
		<html>
			<head>
				<script language="javascript">
          var ie4 = false; if(document.all) { ie4 = true; }
          function getObject(id) { if (ie4) { return document.all[id]; } else { return document.getElementById(id); } }
          function toggle(link, divId) { var lText = link.innerHTML; var d = getObject(divId);
          if (lText == &apos;+&apos;) { link.innerHTML = &apos;-&apos;; d.style.display = &apos;block&apos;; }
          else { link.innerHTML = &apos;+&apos;; d.style.display = &apos;none&apos;; } }
				</script>
			</head>
			<body bgcolor="#C0C0C0">
				<h1 align="center">
          CS3203 - Analysis of the Testing for Team <xsl:value-of select="./info/name/text()"/>
				</h1>
				<br/>
				<table border="3" id="summarytable" width="100%">
					<tr>
						<td width="30%">
              <i>
                Team <xsl:value-of select="./info/name/text()"/>
              </i>
						</td>
						<td width="40%">
              Build Config: <i>
              <xsl:value-of select="./info/build/text()"/>
            </i>
            </td>
						<td width="30%">
              Date: 
              <i>
              <xsl:value-of select="./info/datetime/text()"/>
              </i>
            </td>
					</tr>
				</table>
				<br/>
				<table border="3" height="200" id="table3" width="100%" cellspacing="6">
					<tr>
						<td colspan="3">
							<center>
								<b>Summary</b>
							</center>
						</td>
					</tr>
					<!--	
								<tr>
									<td width="20%">Team # <xsl:value-of select="./info/name/text()"/></td>
									<td width="5%">Comments</td>
									<td width="75%">Date:     Time: </td>
								</tr>
							-->
					<tr>
						<td width="20%">
							<b>AVE-PERF:</b>
						</td>
						<td width="10%"><xsl:text>   </xsl:text></td>
						<td width="70%"> Average Performance computed only for test cases that passed</td>
					</tr>
					<tr>
						<td width="20%">
							<b>Total-TC: </b>
						</td>
						<td width="10%"><xsl:value-of select="count(//query)"/>
						</td>
						<td width="70%">Total number of test cases run by Autotester (including CRASH)</td>
					</tr>
					<tr>
						<td width="20%">
							<b>TE: </b>
						</td>
						<td width="10%"><xsl:value-of select="count(//failed)+count(//timeout)+count(//exception)+count(//crash)"/>
						</td>
						<td width="70%">Total number of test cases that failed (WR+TME+CRASH+EXCEPT)</td>
					</tr>
					<tr>
						<td width="20%">
							<b>PASS: </b>
						</td>
						<td width="10%"><xsl:value-of select="count(//passed)"/>
						</td>
						<td width="70%">Total number of test cases that produced correct result (TOTAL-TC-TE)</td>
					</tr>
					<tr>
						<td width="20%">
							<b>WR: </b>
						</td>
						<td width="10%"><xsl:value-of select="count(//failed)"/>
						</td>
						<td width="70%">Total number of tests that run ok but produced wrong results</td>
					</tr>
					<tr>
						<td colspan="3">
										List WR test case numbers: 			  <br/>
							<xsl:for-each select=".//query[./failed]">
                     							 [                                           
								<a href="#{./id}">
									<xsl:value-of select="./id"/>
								</a>
                    							  ]
							</xsl:for-each>
						</td>
					</tr>
          <!--
					<tr>
						<td width="20%">
							<b>UE: </b>
						</td>
						<td width="10%"/>
						<td width="70%">Total number of unique errors, i.e., groups of test cases (WR+TME+CRASH+EXCEPT) <br/>  that most likely failed for the same reason; entered manually after analysis</td>
					</tr>
          -->
					<tr>
						<td colspan="3">
										Notes: <i>
              <xsl:value-of select="./info/notes/text()"/>
            </i>
						</td>
					</tr>
					<tr>
						<td width="20%">
							<b>TME: </b>
						</td>
						<td width="10%"><xsl:value-of select="count(//timeout)"/>
						</td>
						<td width="70%">Total number of tests that timeout in extended time</td>
					</tr>
					<tr>
						<td colspan="3">
										List TME test case numbers:    <br/>
							<xsl:for-each select=".//query[./timeout]">
                     							 [                                           
								<a href="#{./id}">
									<xsl:value-of select="./id"/>
								</a>
                     							 ]
							</xsl:for-each>
						</td>
					</tr>
					<tr>
						<td width="20%">
							<b>CRASH:  </b>
						</td>
						<td width="10%"><xsl:value-of select="count(//crash)"/>
						</td>
						<td width="70%">Total number of tests that crashed</td>
					</tr>
					<tr>
						<td colspan="3">
										List CRASH test case numbers:         <br/>
							<xsl:for-each select=".//query[./crash]">
                     							 [                                           
								<a href="#{./id}">
									<xsl:value-of select="./id"/>
								</a>
                     							 ]
							</xsl:for-each>
						</td>
					</tr>
					<tr>
						<td width="20%">
							<b>EXCEPT: </b>
						</td>
						<td width="10%"><xsl:value-of select="count(//exception)"/>
						</td>
						<td width="70%">Total number of tests that raised exception</td>
					</tr>
					<tr>
						<td colspan="3">
										List EXCEPT test case numbers:          <br/>
							<xsl:for-each select=".//query[./exception]">
                     							 [                                           
								<a href="#{./id}">
									<xsl:value-of select="./id"/>
								</a>
                     							 ]
							</xsl:for-each>
						</td>
					</tr>
				</table>
				<p/>
				<xsl:apply-templates select="./queries"/>
				
				<table border="0" id="tagstable" width="100%">
					<tr>
						<td width="500">
							<table border="1" height="174" id="table3" width="66%">
								<tr>
									<td colspan="2">
										<b>View By Tags</b>
									</td>
								</tr>
								<xsl:call-template name="printTags">
									<xsl:with-param name="printedTags" select="''"/>
								</xsl:call-template>
								<tr>
									<td>
                    Non Passed                                        <br/>
										<xsl:for-each select=".//query[not(./passed)]">
                      [                                            <a href="#{./id}"><xsl:value-of select="./id"/>
											</a>
                      ]
										</xsl:for-each>
									</td>
									<td width="46">
										<xsl:value-of select="count(//failed)+count(//timeout)+count(//exception)"/>
									</td>
								</tr>
							</table>
						</td>
						<td width="104"/>
						<td/>
					</tr>
				</table>
				
				<p/>
			</body>
		</html>
	</xsl:template>
	<xsl:template match="queries">
		<hr size="4"/>
		<br/>
		<h3>
			<xsl:value-of select="@name"/>
		</h3>
		<xsl:for-each select=".//query">
			<a name="{./id}"/>
			<xsl:choose>
				<xsl:when test="./passed">
					<div style="border: 1px solid #000000; padding: 0px; background: #EEEEEE;">
						<table border="0" cellpadding="2" cellspacing="0" style="background: green; color: #FFFFFF;" width="100%">
							<tr>
								<td>
									<b>
										<xsl:value-of select="./id"/>   [passed]
									</b>
									<xsl:text disable-output-escaping="yes">  Tags: </xsl:text>
									<xsl:for-each select="./id/@*[name()!=$comment]">
                    [<xsl:value-of select="name()"/>]
										<xsl:text disable-output-escaping="yes"/>
									</xsl:for-each>
									<em>
										<b>
											<xsl:text disable-output-escaping="yes"></xsl:text>
										</b>
										<xsl:value-of select="./id/@comment"/>
									</em>
								</td>
								<td align="right">
                  [                                    <a href="javascript: void(0);" id="exp{./id}_link" onclick="toggle(this, &apos;exp{./id}&apos;);" style="text-decoration: none; color: #FFFFFF;" title="show/hide">-</a>
                  ]
								</td>
							</tr>
						</table>
						<div id="exp{./id}" style="padding: 3px;">
							<table bgcolor="white" border="1" id="table1" width="100%">
								<xsl:apply-templates select="querystr"/>
								<!--xsl:apply-templates select="query"/>
										<xsl:apply-templates select="explanation"/-->
							</table>
						</div>
					</div>
					<xsl:if test="./passed">
						<script language="javascript">
              toggle(getObject(&apos;exp                            <xsl:value-of select="./id"/>
              _link&apos;), &apos;exp                            <xsl:value-of select="./id"/>
              &apos;);
						</script>
					</xsl:if>
				</xsl:when>
				<xsl:when test="./timeout">
					<div style="border: 1px solid #000000; padding: 0px; background: #EEEEEE;">
						<table border="0" cellpadding="2" cellspacing="0" style="background: blue; color: #FFFFFF;" width="100%">
							<tr>
								<td>
									<b>
										<xsl:value-of select="./id"/>   [timeout]
									</b>
									<xsl:text disable-output-escaping="yes">  Tags: </xsl:text>
									<xsl:for-each select="./id/@*[name()!=$comment]">
                    [<xsl:value-of select="name()"/>]
										<xsl:text disable-output-escaping="yes"/>
									</xsl:for-each>
									<em>
										<b>
											<xsl:text disable-output-escaping="yes">  Comment: </xsl:text>
										</b>
										<xsl:value-of select="./id/@comment"/>
									</em>
								</td>
								<td align="right">
                  [                                    <a href="javascript: void(0);" id="exp{./id}_link" onclick="toggle(this, &apos;exp{./id}&apos;);" style="text-decoration: none; color: #FFFFFF;" title="show/hide">-</a>
                  ]
								</td>
							</tr>
						</table>
						<div id="exp{./id}" style="padding: 3px;">
							<table bgcolor="white" border="1" id="table1" width="100%">
								<xsl:apply-templates select="querystr"/>
								<!--xsl:apply-templates select="query"/>
										<xsl:apply-templates select="explanation"/-->
							</table>
						</div>
					</div>
					<xsl:if test="./passed">
						<script language="javascript">
              toggle(getObject(&apos;exp                            <xsl:value-of select="./id"/>
              _link&apos;), &apos;exp                            <xsl:value-of select="./id"/>
              &apos;);
						</script>
					</xsl:if>
				</xsl:when>
				<xsl:when test="./exception">
					<div style="border: 1px solid #000000; padding: 0px; background: #EEEEEE;">
						<table border="0" cellpadding="2" cellspacing="0" style="background: brown; color: #FFFFFF;" width="100%">
							<tr>
								<td>
									<b>
										<xsl:value-of select="./id"/>   [exception]
									</b>
									<xsl:text disable-output-escaping="yes">  Tags: </xsl:text>
									<xsl:for-each select="./id/@*[name()!=$comment]">
                    [<xsl:value-of select="name()"/>]
										<xsl:text disable-output-escaping="yes"/>
									</xsl:for-each>
									<em>
										<b>
											<xsl:text disable-output-escaping="yes">  Comment: </xsl:text>
										</b>
										<xsl:value-of select="./id/@comment"/>
									</em>
								</td>
								<td align="right">
                  [                                    <a href="javascript: void(0);" id="exp{./id}_link" onclick="toggle(this, &apos;exp{./id}&apos;);" style="text-decoration: none; color: #FFFFFF;" title="show/hide">-</a>
                  ]
								</td>
							</tr>
						</table>
						<div id="exp{./id}" style="padding: 3px;">
							<table bgcolor="white" border="1" id="table1" width="100%">
								<xsl:apply-templates select="querystr"/>
								<!--xsl:apply-templates select="query"/>
										<xsl:apply-templates select="explanation"/-->
							</table>
						</div>
					</div>
					<xsl:if test="./passed">
						<script language="javascript">
              toggle(getObject(&apos;exp                            <xsl:value-of select="./id"/>
              _link&apos;), &apos;exp                            <xsl:value-of select="./id"/>
              &apos;);
						</script>
					</xsl:if>
				</xsl:when>
				<xsl:when test="./crash">
					<div style="border: 1px solid #000000; padding: 0px; background: #EEEEEE;">
						<table border="0" cellpadding="2" cellspacing="0" style="background: black; color: #FFFFFF;" width="100%">
							<tr>
								<td>
									<b>
										<xsl:value-of select="./id"/>   [crash]
									</b>
									<xsl:text disable-output-escaping="yes">  Tags: </xsl:text>
									<xsl:for-each select="./id/@*[name()!=$comment]">
                    [<xsl:value-of select="name()"/>]
										<xsl:text disable-output-escaping="yes"/>
									</xsl:for-each>
									<em>
										<b>
											<xsl:text disable-output-escaping="yes">  Comment: </xsl:text>
										</b>
										<xsl:value-of select="./id/@comment"/>
									</em>
								</td>
								<td align="right">
                  [                                    <a href="javascript: void(0);" id="exp{./id}_link" onclick="toggle(this, &apos;exp{./id}&apos;);" style="text-decoration: none; color: #FFFFFF;" title="show/hide">-</a>
                  ]
								</td>
							</tr>
						</table>
						<div id="exp{./id}" style="padding: 3px;">
							<table bgcolor="white" border="1" id="table1" width="100%">
								<xsl:apply-templates select="querystr"/>
								<!--xsl:apply-templates select="query"/>
										<xsl:apply-templates select="explanation"/-->
							</table>
						</div>
					</div>
					<xsl:if test="./passed">
						<script language="javascript">
              toggle(getObject(&apos;exp                            <xsl:value-of select="./id"/>
              _link&apos;), &apos;exp                            <xsl:value-of select="./id"/>
              &apos;);
						</script>
					</xsl:if>
				</xsl:when>
				<xsl:otherwise>
					<div style="border: 1px solid #000000; padding: 0px; background: #EEEEEE;">
						<table border="0" cellpadding="2" cellspacing="0" style="background: red; color: #FFFFFF;" width="100%">
							<tr>
								<td>
									<b>
										<xsl:value-of select="./id"/>   [failed]
									</b>
									<xsl:text disable-output-escaping="yes">  Tags: </xsl:text>
									<xsl:for-each select="./id/@*[name()!=$comment]">
                    [<xsl:value-of select="name()"/>]
										<xsl:text disable-output-escaping="yes"/>
									</xsl:for-each>
									<em>
										<b>
											<xsl:text disable-output-escaping="yes">  Comment: </xsl:text>
										</b>
										<xsl:value-of select="./id/@comment"/>
									</em>
								</td>
								<td align="right">
                  [                                    <a href="javascript: void(0);" id="exp{./id}_link" onclick="toggle(this, &apos;exp{./id}&apos;);" style="text-decoration: none; color: #FFFFFF;" title="show/hide">-</a>
                  ]
								</td>
							</tr>
						</table>
						<div id="exp{./id}" style="padding: 3px;">
							<table bgcolor="white" border="1" id="table1" width="100%">
								<xsl:apply-templates select="querystr"/>
								<xsl:apply-templates select="exception"/>
								<xsl:apply-templates select="crash"/>
								<xsl:apply-templates select="failed"/>
								<!--xsl:apply-templates select="define"/>
										<xsl:apply-templates select="query"/>
										<xsl:apply-templates select="explanation"/-->
							</table>
						</div>
					</div>
					<xsl:if test="./passed">
						<script language="javascript">
              toggle(getObject(&apos;exp                            <xsl:value-of select="./id"/>
              _link&apos;), &apos;exp                            <xsl:value-of select="./id"/>
              &apos;);
						</script>
					</xsl:if>
				</xsl:otherwise>
			</xsl:choose>
			<br/>
		</xsl:for-each>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="define" name="define">
		<tr>
			<xsl:if test="../passed">
				<td width="80%">
					<font color="green">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>
						<xsl:value-of select="@run"/>
					</pre>
				</td>
				<td width="15%">
					<pre>
						<xsl:value-of select="@time"/>
					</pre>
				</td>
			</xsl:if>
			<xsl:if test="../failed">
				<td width="80%">
					<font color="red">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>
						<xsl:value-of select="@run"/>
					</pre>
				</td>
				<td width="15%">
					<pre>
						<xsl:value-of select="@time"/>
					</pre>
				</td>
			</xsl:if>
			<xsl:if test="(@run = &apos;timeout&apos;)">
				<td width="80%">
					<font color="blue">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>
						<xsl:value-of select="@run"/>
					</pre>
				</td>
				<td width="15%">-</td>
			</xsl:if>
			<xsl:if test="(@run = &apos;exception&apos;)">
				<td width="80%">
					<font color="brown">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>
						<xsl:value-of select="@run"/>
					</pre>
				</td>
				<td width="15%">-</td>
			</xsl:if>
			<xsl:if test="(@run = &apos;crash&apos;)">
				<td width="80%">
					<font color="black">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>
						<xsl:value-of select="@run"/>
					</pre>
				</td>
				<td width="15%">-</td>
			</xsl:if>
			<xsl:if test="(@run = &apos;never&apos;)">
				<td width="80%">
					<font color="gray">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>
						<xsl:value-of select="@run"/>
					</pre>
				</td>
				<td width="15%">-</td>
			</xsl:if>
		</tr>
	</xsl:template>
	<xsl:template match="querystr" name="querystr">
		<tr>
			<xsl:if test="../passed">
				<td width="80%">
					<font color="green">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>passed</pre>
				</td>
				<td width="15%">
					<pre>
						<xsl:value-of select="../time_taken"/>
					</pre>
				</td>
			</xsl:if>
			<xsl:if test="../failed">
				<td width="80%">
					<font color="red">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>failed</pre>
				</td>
				<td width="15%">
					<pre>
						<xsl:value-of select="../time_taken"/>
					</pre>
				</td>
			</xsl:if>
			<xsl:if test="../timeout">
				<td width="80%">
					<font color="blue">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>timeout</pre>
				</td>
				<td width="15%">-</td>
			</xsl:if>
			<xsl:if test="../exception">
				<td width="80%">
					<font color="brown">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>exception</pre>
				</td>
				<td width="15%">-</td>
			</xsl:if>
			<xsl:if test="../crash">
				<td width="80%">
					<font color="black">
						<b>
							<pre>
								<xsl:value-of select="."/>
							</pre>
						</b>
					</font>
				</td>
				<td width="5%">
					<pre>crash</pre>
				</td>
				<td width="15%">-</td>
			</xsl:if>
		</tr>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="query">
		<xsl:call-template name="define"/>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="desc">
    [        <xsl:value-of select="."/>
    ]
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="exception">
		<table bgcolor="white" border="1" id="table3" width="100%">
			<tr>
				<td width="100%">
					<b>
						<pre>Exception :</pre>
					</b>
					<pre>
						<xsl:apply-templates select="text()"/>
					</pre>
				</td>
			</tr>
		</table>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="crash">
		<table bgcolor="white" border="1" id="table3" width="100%">
			<tr>
				<td width="100%">
					<b>
						<pre>Crash :</pre>
					</b>
					<pre>
						<xsl:apply-templates select="text()"/>
					</pre>
				</td>
			</tr>
		</table>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="failed">
		<table bgcolor="white" border="1" id="table2" width="100%">
			<tr>
				<td>
					<b>
						<pre>Instance missing from result :</pre>
					</b>
					<xsl:apply-templates select="missing"/>
				</td>
			</tr>
			<tr>
				<td>
					<b>
						<pre>Extra answers reported :</pre>
					</b>
					<xsl:apply-templates select="additional"/>
				</td>
			</tr>
			<tr>
				<td>
					<b>
						<pre>Summary :</pre>
					</b>
					<xsl:apply-templates select="summary"/>
				</td>
			</tr>
		</table>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="additional" name="additional">
		<xsl:value-of select="."/>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="missing" name="missing">
		<xsl:value-of select="."/>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="summary">
		<b>
			<pre>
        Expected:                <xsl:value-of select="./expected"/>
        Matched:                <xsl:value-of select="./matched"/>
        Extra:                <xsl:value-of select="./additional"/>
        Missing:                <xsl:value-of select="./missing"/>
			</pre>
		</b>
	</xsl:template>
	<!-- ============================ -->
	<!-- ============================ -->
	<xsl:template match="explanation">
		<xsl:apply-templates select="msg"/>
		<xsl:apply-templates select="indComparison"/>
		<xsl:apply-templates select="patComparison"/>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="msg" name="msg">
		<table bgcolor="white" border="1" id="table3" width="100%">
			<tr>
				<td width="100%">
					<b>
						<pre>Error :</pre>
					</b>
					<pre>
						<xsl:apply-templates select="text()"/>
					</pre>
				</td>
			</tr>
		</table>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="indComparison">
		<table bgcolor="white" border="1" id="table2" width="100%">
			<tr>
				<td width="12%">
					<b>DATE</b>
				</td>
				<td width="13%">
					<b>OPEN</b>
				</td>
				<td width="12%">
					<b>CLOSE</b>
				</td>
				<td width="13%">
					<b>HIGH</b>
				</td>
				<td width="12%">
					<b>LOW</b>
				</td>
				<td width="13%">
					<b>VOLUME</b>
				</td>
				<td width="12%">
					<b>EXPECTED</b>
				</td>
				<td width="13%">
					<b>ACTUAL</b>
				</td>
			</tr>
			<xsl:apply-templates select="indMismatch"/>
		</table>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="indMismatch" name="indMismatch">
		<tr height="10">
			<td width="12%">
				<pre>
					<xsl:value-of select="@date"/>
				</pre>
			</td>
			<td width="13%">
				<pre>
					<xsl:value-of select="@open"/>
				</pre>
			</td>
			<td width="12%">
				<pre>
					<xsl:value-of select="@close"/>
				</pre>
			</td>
			<td width="13%">
				<pre>
					<xsl:value-of select="@high"/>
				</pre>
			</td>
			<td width="12%">
				<pre>
					<xsl:value-of select="@low"/>
				</pre>
			</td>
			<td width="13%">
				<pre>
					<xsl:value-of select="@volume"/>
				</pre>
			</td>
			<td width="12%">
				<pre>
					<xsl:value-of select="@expected"/>
				</pre>
			</td>
			<td width="13%">
				<pre>
					<xsl:value-of select="@actual"/>
				</pre>
			</td>
		</tr>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="patComparison">
		<table bgcolor="white" border="1" id="table2" width="100%">
			<tr>
				<td>
					<b>
						<pre>Instance missing from result :</pre>
					</b>
					<xsl:apply-templates select="missing"/>
				</td>
			</tr>
			<tr>
				<td>
					<b>
						<pre>Extra answers reported :</pre>
					</b>
					<xsl:apply-templates select="extra"/>
				</td>
			</tr>
			<tr>
				<td>
					<b>
						<pre>Summary :</pre>
					</b>
					<xsl:apply-templates select="stat"/>
				</td>
			</tr>
		</table>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="TestCase" name="failedList">
		<xsl:value-of select="@name"/>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="extra" name="extra">
		<xsl:value-of select="."/>
	</xsl:template>
	<!-- ============================ -->
	<!-- ============================ -->
	<!-- ============================ -->
	<xsl:template match="comment()">
		<i>
      ha            <xsl:value-of select="."/>
		</i>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template match="text()">
		<xsl:call-template name="break"/>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template name="break">
		<xsl:param name="text" select="."/>
		<xsl:choose>
			<xsl:when test="contains($text, &apos; &apos;)">
				<xsl:value-of select="substring-before($text, &apos; &apos;)"/>
				<br/>
				<xsl:call-template name="break">
					<xsl:with-param name="text" select="substring-after($text,&apos; &apos;)"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$text"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!-- ============================ -->
	<xsl:template name="printTags">
		<xsl:param name="printedTags"/>
		<xsl:variable name="attrName">
			<xsl:value-of select="name(//@*[name()!=$comment and name()!='name' and not(contains($printedTags,concat(':: ',name(),'::'))) ])"/>
		</xsl:variable>
		<xsl:if test="not(contains($printedTags,concat(':: ',$attrName,'::')))">
			<tr>
				<td>
					<xsl:value-of select="$attrName"/>
					<!--xsl:value-of select="$printedTags"/-->
					<br/>
					<xsl:for-each select="//query[./id[@*[name() =$attrName]] and (not(./passed))]">
            [                                            <a href="#{./id}"><xsl:value-of select="./id"/>
						</a>
            ]
					</xsl:for-each>
				</td>
				<td width="46">
					<xsl:value-of select="count(//query[./id[@*[name() =$attrName]] and (not(./passed))])"/>
				</td>
			</tr>
			<xsl:call-template name="printTags">
				<xsl:with-param name="printedTags" select="concat($printedTags,':: ',$attrName,'::')"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>
