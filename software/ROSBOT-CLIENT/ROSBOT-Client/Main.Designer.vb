<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Main
    Inherits System.Windows.Forms.Form

    'Form esegue l'override del metodo Dispose per pulire l'elenco dei componenti.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Richiesto da Progettazione Windows Form
    Private components As System.ComponentModel.IContainer

    'NOTA: la procedura che segue è richiesta da Progettazione Windows Form
    'Può essere modificata in Progettazione Windows Form.  
    'Non modificarla mediante l'editor del codice.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Main))
        Me.btnListen = New System.Windows.Forms.Button()
        Me.btnConnect = New System.Windows.Forms.Button()
        Me.txtIP = New System.Windows.Forms.TextBox()
        Me.txtMessage = New System.Windows.Forms.TextBox()
        Me.txtLog = New System.Windows.Forms.TextBox()
        Me.btnSend = New System.Windows.Forms.Button()
        Me.btnDisconnect = New System.Windows.Forms.Button()
        Me.tmrData = New System.Windows.Forms.Timer(Me.components)
        Me.tmrConnections = New System.Windows.Forms.Timer(Me.components)
        Me.comboComando = New System.Windows.Forms.ComboBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.txtX = New System.Windows.Forms.TextBox()
        Me.txtY = New System.Windows.Forms.TextBox()
        Me.txtSpeed = New System.Windows.Forms.TextBox()
        Me.txtZCode = New System.Windows.Forms.TextBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.chkGripper = New System.Windows.Forms.CheckBox()
        Me.btnSendAll = New System.Windows.Forms.Button()
        Me.lvComandi = New System.Windows.Forms.ListView()
        Me.Comando = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.cmdAdd = New System.Windows.Forms.Button()
        Me.lblStato = New System.Windows.Forms.Label()
        Me.tmrCiclo = New System.Windows.Forms.Timer(Me.components)
        Me.cmdCiclo = New System.Windows.Forms.Button()
        Me.cmdClearList = New System.Windows.Forms.Button()
        Me.cmdEmr = New System.Windows.Forms.Button()
        Me.cmdReset = New System.Windows.Forms.Button()
        Me.txtDelay = New System.Windows.Forms.TextBox()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.txtAccel = New System.Windows.Forms.TextBox()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.chkManual = New System.Windows.Forms.CheckBox()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.numStepManuale = New System.Windows.Forms.NumericUpDown()
        Me.cmdYpositive = New System.Windows.Forms.Button()
        Me.cmdYnegative = New System.Windows.Forms.Button()
        Me.cmdXnegative = New System.Windows.Forms.Button()
        Me.cmdXpositive = New System.Windows.Forms.Button()
        Me.lblCurrentCommand = New System.Windows.Forms.Label()
        CType(Me.numStepManuale, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'btnListen
        '
        Me.btnListen.Location = New System.Drawing.Point(41, 60)
        Me.btnListen.Name = "btnListen"
        Me.btnListen.Size = New System.Drawing.Size(27, 18)
        Me.btnListen.TabIndex = 0
        Me.btnListen.Text = "Server Ascolta"
        Me.btnListen.UseVisualStyleBackColor = True
        '
        'btnConnect
        '
        Me.btnConnect.Location = New System.Drawing.Point(1031, 12)
        Me.btnConnect.Name = "btnConnect"
        Me.btnConnect.Size = New System.Drawing.Size(110, 51)
        Me.btnConnect.TabIndex = 1
        Me.btnConnect.Text = "Connetti Client"
        Me.btnConnect.UseVisualStyleBackColor = True
        '
        'txtIP
        '
        Me.txtIP.Location = New System.Drawing.Point(1031, 69)
        Me.txtIP.Name = "txtIP"
        Me.txtIP.Size = New System.Drawing.Size(110, 22)
        Me.txtIP.TabIndex = 2
        '
        'txtMessage
        '
        Me.txtMessage.Location = New System.Drawing.Point(12, 12)
        Me.txtMessage.Multiline = True
        Me.txtMessage.Name = "txtMessage"
        Me.txtMessage.Size = New System.Drawing.Size(353, 77)
        Me.txtMessage.TabIndex = 3
        '
        'txtLog
        '
        Me.txtLog.Location = New System.Drawing.Point(12, 97)
        Me.txtLog.Multiline = True
        Me.txtLog.Name = "txtLog"
        Me.txtLog.Size = New System.Drawing.Size(353, 548)
        Me.txtLog.TabIndex = 4
        '
        'btnSend
        '
        Me.btnSend.Location = New System.Drawing.Point(272, 403)
        Me.btnSend.Name = "btnSend"
        Me.btnSend.Size = New System.Drawing.Size(20, 18)
        Me.btnSend.TabIndex = 5
        Me.btnSend.Text = "INVIA"
        Me.btnSend.UseVisualStyleBackColor = True
        '
        'btnDisconnect
        '
        Me.btnDisconnect.Location = New System.Drawing.Point(1031, 97)
        Me.btnDisconnect.Name = "btnDisconnect"
        Me.btnDisconnect.Size = New System.Drawing.Size(110, 51)
        Me.btnDisconnect.TabIndex = 6
        Me.btnDisconnect.Text = "Disconnetti"
        Me.btnDisconnect.UseVisualStyleBackColor = True
        '
        'tmrData
        '
        Me.tmrData.Interval = 20
        '
        'tmrConnections
        '
        '
        'comboComando
        '
        Me.comboComando.FormattingEnabled = True
        Me.comboComando.Items.AddRange(New Object() {"M4", "M5", "M6", "M7", "M8", "S4", "C1"})
        Me.comboComando.Location = New System.Drawing.Point(374, 26)
        Me.comboComando.Name = "comboComando"
        Me.comboComando.Size = New System.Drawing.Size(188, 24)
        Me.comboComando.TabIndex = 7
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(371, 6)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(72, 17)
        Me.Label1.TabIndex = 8
        Me.Label1.Text = "Comando:"
        '
        'txtX
        '
        Me.txtX.Location = New System.Drawing.Point(374, 56)
        Me.txtX.Name = "txtX"
        Me.txtX.Size = New System.Drawing.Size(100, 22)
        Me.txtX.TabIndex = 9
        '
        'txtY
        '
        Me.txtY.Location = New System.Drawing.Point(374, 87)
        Me.txtY.Name = "txtY"
        Me.txtY.Size = New System.Drawing.Size(100, 22)
        Me.txtY.TabIndex = 10
        '
        'txtSpeed
        '
        Me.txtSpeed.Location = New System.Drawing.Point(374, 118)
        Me.txtSpeed.Name = "txtSpeed"
        Me.txtSpeed.Size = New System.Drawing.Size(100, 22)
        Me.txtSpeed.TabIndex = 11
        '
        'txtZCode
        '
        Me.txtZCode.Location = New System.Drawing.Point(374, 149)
        Me.txtZCode.Name = "txtZCode"
        Me.txtZCode.Size = New System.Drawing.Size(100, 22)
        Me.txtZCode.TabIndex = 12
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(481, 59)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(17, 17)
        Me.Label2.TabIndex = 13
        Me.Label2.Text = "X"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(481, 90)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(17, 17)
        Me.Label3.TabIndex = 14
        Me.Label3.Text = "Y"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(481, 121)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(49, 17)
        Me.Label4.TabIndex = 15
        Me.Label4.Text = "Speed"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(481, 245)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(56, 17)
        Me.Label5.TabIndex = 16
        Me.Label5.Text = "Gripper"
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Location = New System.Drawing.Point(481, 152)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(54, 17)
        Me.Label6.TabIndex = 17
        Me.Label6.Text = "Z Code"
        '
        'chkGripper
        '
        Me.chkGripper.AutoSize = True
        Me.chkGripper.Location = New System.Drawing.Point(376, 247)
        Me.chkGripper.Name = "chkGripper"
        Me.chkGripper.Size = New System.Drawing.Size(18, 17)
        Me.chkGripper.TabIndex = 18
        Me.chkGripper.UseVisualStyleBackColor = True
        '
        'btnSendAll
        '
        Me.btnSendAll.Location = New System.Drawing.Point(378, 577)
        Me.btnSendAll.Name = "btnSendAll"
        Me.btnSendAll.Size = New System.Drawing.Size(75, 68)
        Me.btnSendAll.TabIndex = 19
        Me.btnSendAll.Text = "INVIA"
        Me.btnSendAll.UseVisualStyleBackColor = True
        '
        'lvComandi
        '
        Me.lvComandi.Columns.AddRange(New System.Windows.Forms.ColumnHeader() {Me.Comando})
        Me.lvComandi.HideSelection = False
        Me.lvComandi.Location = New System.Drawing.Point(681, 12)
        Me.lvComandi.Name = "lvComandi"
        Me.lvComandi.Size = New System.Drawing.Size(317, 633)
        Me.lvComandi.TabIndex = 20
        Me.lvComandi.UseCompatibleStateImageBehavior = False
        Me.lvComandi.View = System.Windows.Forms.View.Details
        '
        'Comando
        '
        Me.Comando.Text = "Comando"
        Me.Comando.Width = 230
        '
        'cmdAdd
        '
        Me.cmdAdd.Location = New System.Drawing.Point(1031, 427)
        Me.cmdAdd.Name = "cmdAdd"
        Me.cmdAdd.Size = New System.Drawing.Size(110, 68)
        Me.cmdAdd.TabIndex = 21
        Me.cmdAdd.Text = "Aggiungi"
        Me.cmdAdd.UseVisualStyleBackColor = True
        '
        'lblStato
        '
        Me.lblStato.AutoSize = True
        Me.lblStato.Location = New System.Drawing.Point(376, 321)
        Me.lblStato.Name = "lblStato"
        Me.lblStato.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblStato.Size = New System.Drawing.Size(0, 17)
        Me.lblStato.TabIndex = 22
        '
        'cmdCiclo
        '
        Me.cmdCiclo.Location = New System.Drawing.Point(1031, 502)
        Me.cmdCiclo.Name = "cmdCiclo"
        Me.cmdCiclo.Size = New System.Drawing.Size(110, 68)
        Me.cmdCiclo.TabIndex = 23
        Me.cmdCiclo.Text = "StartCiclo"
        Me.cmdCiclo.UseVisualStyleBackColor = True
        '
        'cmdClearList
        '
        Me.cmdClearList.Location = New System.Drawing.Point(1031, 577)
        Me.cmdClearList.Name = "cmdClearList"
        Me.cmdClearList.Size = New System.Drawing.Size(110, 68)
        Me.cmdClearList.TabIndex = 24
        Me.cmdClearList.Text = "Clear"
        Me.cmdClearList.UseVisualStyleBackColor = True
        '
        'cmdEmr
        '
        Me.cmdEmr.Location = New System.Drawing.Point(517, 577)
        Me.cmdEmr.Name = "cmdEmr"
        Me.cmdEmr.Size = New System.Drawing.Size(75, 68)
        Me.cmdEmr.TabIndex = 25
        Me.cmdEmr.Text = "EMR!"
        Me.cmdEmr.UseVisualStyleBackColor = True
        '
        'cmdReset
        '
        Me.cmdReset.Location = New System.Drawing.Point(600, 577)
        Me.cmdReset.Name = "cmdReset"
        Me.cmdReset.Size = New System.Drawing.Size(75, 68)
        Me.cmdReset.TabIndex = 26
        Me.cmdReset.Text = "RESET"
        Me.cmdReset.UseVisualStyleBackColor = True
        '
        'txtDelay
        '
        Me.txtDelay.Location = New System.Drawing.Point(374, 180)
        Me.txtDelay.Name = "txtDelay"
        Me.txtDelay.Size = New System.Drawing.Size(100, 22)
        Me.txtDelay.TabIndex = 27
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Location = New System.Drawing.Point(481, 183)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(44, 17)
        Me.Label7.TabIndex = 28
        Me.Label7.Text = "Delay"
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Location = New System.Drawing.Point(481, 214)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(42, 17)
        Me.Label8.TabIndex = 30
        Me.Label8.Text = "Accel"
        '
        'txtAccel
        '
        Me.txtAccel.Location = New System.Drawing.Point(374, 211)
        Me.txtAccel.Name = "txtAccel"
        Me.txtAccel.Size = New System.Drawing.Size(100, 22)
        Me.txtAccel.TabIndex = 29
        '
        'Button1
        '
        Me.Button1.Location = New System.Drawing.Point(1031, 154)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(110, 48)
        Me.Button1.TabIndex = 31
        Me.Button1.Text = "Importa"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Location = New System.Drawing.Point(1004, 72)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(24, 17)
        Me.Label9.TabIndex = 32
        Me.Label9.Text = "IP:"
        '
        'chkManual
        '
        Me.chkManual.AutoSize = True
        Me.chkManual.Location = New System.Drawing.Point(376, 354)
        Me.chkManual.Name = "chkManual"
        Me.chkManual.Size = New System.Drawing.Size(18, 17)
        Me.chkManual.TabIndex = 34
        Me.chkManual.UseVisualStyleBackColor = True
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Location = New System.Drawing.Point(405, 354)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(132, 17)
        Me.Label10.TabIndex = 33
        Me.Label10.Text = "ABILITA MANUALE:"
        '
        'numStepManuale
        '
        Me.numStepManuale.Location = New System.Drawing.Point(543, 351)
        Me.numStepManuale.Name = "numStepManuale"
        Me.numStepManuale.Size = New System.Drawing.Size(74, 22)
        Me.numStepManuale.TabIndex = 35
        '
        'cmdYpositive
        '
        Me.cmdYpositive.Location = New System.Drawing.Point(493, 392)
        Me.cmdYpositive.Name = "cmdYpositive"
        Me.cmdYpositive.Size = New System.Drawing.Size(50, 49)
        Me.cmdYpositive.TabIndex = 36
        Me.cmdYpositive.Text = "Y+"
        Me.cmdYpositive.UseVisualStyleBackColor = True
        '
        'cmdYnegative
        '
        Me.cmdYnegative.Location = New System.Drawing.Point(493, 506)
        Me.cmdYnegative.Name = "cmdYnegative"
        Me.cmdYnegative.Size = New System.Drawing.Size(50, 49)
        Me.cmdYnegative.TabIndex = 37
        Me.cmdYnegative.Text = "Y-"
        Me.cmdYnegative.UseVisualStyleBackColor = True
        '
        'cmdXnegative
        '
        Me.cmdXnegative.Location = New System.Drawing.Point(436, 448)
        Me.cmdXnegative.Name = "cmdXnegative"
        Me.cmdXnegative.Size = New System.Drawing.Size(50, 49)
        Me.cmdXnegative.TabIndex = 38
        Me.cmdXnegative.Text = "X-"
        Me.cmdXnegative.UseVisualStyleBackColor = True
        '
        'cmdXpositive
        '
        Me.cmdXpositive.Location = New System.Drawing.Point(551, 448)
        Me.cmdXpositive.Name = "cmdXpositive"
        Me.cmdXpositive.Size = New System.Drawing.Size(50, 49)
        Me.cmdXpositive.TabIndex = 39
        Me.cmdXpositive.Text = "X+"
        Me.cmdXpositive.UseVisualStyleBackColor = True
        '
        'lblCurrentCommand
        '
        Me.lblCurrentCommand.AutoSize = True
        Me.lblCurrentCommand.Location = New System.Drawing.Point(376, 276)
        Me.lblCurrentCommand.Name = "lblCurrentCommand"
        Me.lblCurrentCommand.Size = New System.Drawing.Size(0, 17)
        Me.lblCurrentCommand.TabIndex = 40
        '
        'Main
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1153, 657)
        Me.Controls.Add(Me.lblCurrentCommand)
        Me.Controls.Add(Me.cmdXpositive)
        Me.Controls.Add(Me.cmdXnegative)
        Me.Controls.Add(Me.cmdYnegative)
        Me.Controls.Add(Me.cmdYpositive)
        Me.Controls.Add(Me.numStepManuale)
        Me.Controls.Add(Me.chkManual)
        Me.Controls.Add(Me.Label10)
        Me.Controls.Add(Me.Label9)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.Label8)
        Me.Controls.Add(Me.txtAccel)
        Me.Controls.Add(Me.Label7)
        Me.Controls.Add(Me.txtDelay)
        Me.Controls.Add(Me.cmdReset)
        Me.Controls.Add(Me.cmdEmr)
        Me.Controls.Add(Me.cmdClearList)
        Me.Controls.Add(Me.cmdCiclo)
        Me.Controls.Add(Me.lblStato)
        Me.Controls.Add(Me.cmdAdd)
        Me.Controls.Add(Me.lvComandi)
        Me.Controls.Add(Me.btnSendAll)
        Me.Controls.Add(Me.chkGripper)
        Me.Controls.Add(Me.Label6)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.txtZCode)
        Me.Controls.Add(Me.txtSpeed)
        Me.Controls.Add(Me.txtY)
        Me.Controls.Add(Me.txtX)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.comboComando)
        Me.Controls.Add(Me.btnDisconnect)
        Me.Controls.Add(Me.txtLog)
        Me.Controls.Add(Me.txtMessage)
        Me.Controls.Add(Me.txtIP)
        Me.Controls.Add(Me.btnConnect)
        Me.Controls.Add(Me.btnListen)
        Me.Controls.Add(Me.btnSend)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "Main"
        Me.Text = "ROSBOT-Client"
        CType(Me.numStepManuale, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents btnListen As Button
    Friend WithEvents btnConnect As Button
    Friend WithEvents txtIP As TextBox
    Friend WithEvents txtMessage As TextBox
    Friend WithEvents txtLog As TextBox
    Friend WithEvents btnSend As Button
    Friend WithEvents btnDisconnect As Button
    Friend WithEvents tmrData As Timer
    Friend WithEvents tmrConnections As Timer
    Friend WithEvents comboComando As ComboBox
    Friend WithEvents Label1 As Label
    Friend WithEvents txtX As TextBox
    Friend WithEvents txtY As TextBox
    Friend WithEvents txtSpeed As TextBox
    Friend WithEvents txtZCode As TextBox
    Friend WithEvents Label2 As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents Label6 As Label
    Friend WithEvents chkGripper As CheckBox
    Friend WithEvents btnSendAll As Button
    Friend WithEvents lvComandi As ListView
    Friend WithEvents Comando As ColumnHeader
    Friend WithEvents cmdAdd As Button
    Friend WithEvents lblStato As Label
    Friend WithEvents tmrCiclo As Timer
    Friend WithEvents cmdCiclo As Button
    Friend WithEvents cmdClearList As Button
    Friend WithEvents cmdEmr As Button
    Friend WithEvents cmdReset As Button
    Friend WithEvents txtDelay As TextBox
    Friend WithEvents Label7 As Label
    Friend WithEvents Label8 As Label
    Friend WithEvents txtAccel As TextBox
    Friend WithEvents Button1 As Button
    Friend WithEvents Label9 As Label
    Friend WithEvents chkManual As CheckBox
    Friend WithEvents Label10 As Label
    Friend WithEvents numStepManuale As NumericUpDown
    Friend WithEvents cmdYpositive As Button
    Friend WithEvents cmdYnegative As Button
    Friend WithEvents cmdXnegative As Button
    Friend WithEvents cmdXpositive As Button
    Friend WithEvents lblCurrentCommand As Label
End Class
