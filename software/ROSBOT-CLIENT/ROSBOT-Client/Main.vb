Imports System.Net.Sockets
Imports System.Text.UTF8Encoding

Public Class Main

    Private Listener As TcpListener
    Private Client As TcpClient
    Private NetStream As NetworkStream
    Private port As Integer = 12000
    Private defaultIP As String = "10.0.119.69"
    Private timeout As Integer = 3000

    Private type As String
    Private XVal As String
    Private YVal As String
    Private speed As String
    Private acceleration As String
    Private zCommand As String
    Private gripper As Integer
    Private delay As String
    Private value As String
    Private actPosition As New Point

    Private Const endCommand As String = "!"
    Private Const emrCommand As String = "S3"
    Private Const resetCommand As String = "S2"
    Private Const emrState As String = "EMERGENZA"
    Private Const resetState As String = "RESET_ESEGUITO"
    Private Const commandExecutedState As String = "COMANDO_ESEGUITO"
    Private Const commandState As String = "COMANDO"
    Private Const errorState As String = "ERRORE"
    Private Const okMessage As String = "$OK"
    Private Const errMessage As String = "$ERR"


    Private startCiclo As Boolean = False
    Private listComandi As New List(Of Comando)
    Private currentCommand As New Comando
    Private ciclo As New Ciclo
    Private state As String = "ATTESA"
    Private isComandoEseguito As Boolean = False
    Private index As Integer = -1
    Private queue_comando As New Queue(Of String)





    'Questa procedura serve per attivare o disattivare i
    'controlli a seconda che si sia connessi oppure no. Serve
    'per impedire che si tenti di inviare un messaggio quando
    'non si è connessi, ad esempio
    Private Sub EnableControls(ByVal Connected As Boolean)
        btnConnect.Enabled = Not Connected
        btnListen.Enabled = Not Connected
        txtIP.Enabled = Not Connected

        btnSend.Enabled = Connected
        txtMessage.Enabled = Connected
        btnDisconnect.Enabled = Connected
        btnSendAll.Enabled = Connected

        If Connected Then
            tmrData.Start()
        Else
            tmrData.Stop()
        End If
    End Sub

    Private Sub btnListen_Click(ByVal sender As Object, ByVal e As EventArgs) Handles btnListen.Click
        'Inizializza il listener e inizia l'ascolto sulla porta
        '5000. Inoltre, attiva il timer per controllare se ci
        'sono connessioni in arrivo. Il timer scatta ogni 100ms
        Listener = New TcpListener(port)
        Listener.Start()
        tmrConnections.Start()
        btnListen.Enabled = False
        btnConnect.Enabled = False
        txtLog.AppendText("Server - in ascolto..." & Environment.NewLine)
    End Sub

    Private Sub tmrConnections_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles tmrConnections.Tick
        'Se ci sono connessioni...
        If Listener.Pending() Then
            'Ferma un attimo il timer
            tmrConnections.Stop()

            'Chiede all'utente se confermare la connessione
            If MessageBox.Show("Rilevato un tentativo di connessione. Accettare?", Me.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) = Windows.Forms.DialogResult.Yes Then
                'Ottiene l'oggetto TcpClient collegato al client
                Client = Listener.AcceptTcpClient()
                'Ferma il listener
                Listener.Stop()
                'Ottiene il network stream
                NetStream = Client.GetStream()
                'E attiva/disattiva i controlli per quando si è connessi
                EnableControls(True)
            Else
                Listener.Stop()
                Listener.Start()
                tmrConnections.Start()
            End If
        End If
    End Sub

    Private Sub btnConnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnConnect.Click
        Dim IP As Net.IPAddress

        'Prima esegue un controllo sull'indirizzo IP per 
        'controllare che sia valido
        If Not Net.IPAddress.TryParse(txtIP.Text, IP) Then
            MessageBox.Show("IP non valido!", Me.Text, MessageBoxButtons.OK, MessageBoxIcon.Exclamation)
            Exit Sub
        End If

        'Quindi inizializza un client e tenta la connessione
        'al dato IP sulla porta 5000
        Client = New TcpClient()
        txtLog.AppendText("Client - tentativo di connessione..." & vbCrLf)
        Try
            Application.DoEvents()
            Client.Connect(IP, port)
        Catch Ex As Exception

        End Try

        'Se la connessione ha avuto successo, ottiene il network
        'stream e agisce sui controlli come nel codice precedente
        If Client.Connected Then
            txtLog.AppendText("Tentativo di connessione riuscito!" & vbCrLf)
            NetStream = Client.GetStream()
            EnableControls(True)
        Else
            txtLog.AppendText("Tentativo di connessione fallito..." & vbCrLf)
        End If
    End Sub

    Private Sub tmrData_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles tmrData.Tick
        'Se ci sono dati disponibili
        Dim cmd As String = ""
        If Client.Available > 0 Then
            'Li legge dallo stream
            Dim Buffer(Client.Available - 1) As Byte
            NetStream.Read(Buffer, 0, Buffer.Length)
            Dim Msg As String = UTF8.GetString(Buffer)

            'If Msg.StartsWith("&") Then 'MESSAGGI DI STATO ROBOT
            For Each n As String In Msg.Split("%")
                If Not n = "" Then
                    queue_comando.Enqueue(n)
                End If
            Next

            ' End If
        End If


        If queue_comando.Count > 0 Then
            cmd = queue_comando.Dequeue()
            If cmd.StartsWith("&") Then
                cmd = cmd.Replace("&", "")
                If cmd.Contains("COMANDO:") Then
                    state = commandState
                Else
                    state = cmd
                End If
                lblStato.Text = cmd
                Select Case state

                    Case emrState
                        ciclo.setStato(False)
                        startCiclo = False
                        cmdCiclo.Text = "AVVIA CICLO"
                        comboComando.Enabled = False
                        cmdEmr.Enabled = False

                    Case resetState
                        ciclo.reset()
                        startCiclo = False
                        cmdCiclo.Text = "AVVIA CICLO"
                        comboComando.Enabled = True
                        cmdEmr.Enabled = True

                    Case commandExecutedState
                        isComandoEseguito = True
                        ciclo.setStato(True)

                        If startCiclo Then
                            If ciclo.getState Then
                                inviaComando(ciclo.getNext)
                            End If
                        End If

                    Case commandState
                        isComandoEseguito = False
                        ciclo.setStato(False)

                    Case errorState
                        ciclo.reset()

                End Select

            Else
                cmd.Replace("$", "")
                txtLog.AppendText("Ricevuto: ")
                txtLog.AppendText(cmd)
                txtLog.AppendText(Environment.NewLine)
            End If
        End If
        lblCurrentCommand.Text = currentCommand.getValue

    End Sub

    Private Sub btnSend_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSend.Click
        If Not String.IsNullOrEmpty(txtMessage.Text) Then
            Dim Buffer() As Byte = UTF8.GetBytes(txtMessage.Text)
            txtLog.AppendText("Inviato: " & txtMessage.Text & Environment.NewLine)
            NetStream.Write(Buffer, 0, Buffer.Length)
            txtMessage.Text = ""
        End If
    End Sub

    Private Sub btnDisconnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnDisconnect.Click
        tmrData.Stop()

        Dim Buffer() As Byte = UTF8.GetBytes("\close\")
        NetStream.Write(Buffer, 0, Buffer.Length)

        Client.Client.Close()
        Client.Close()
        Client = Nothing

        If Listener IsNot Nothing Then
            Listener.Server.Close()
            Listener = Nothing
        End If

        EnableControls(False)
        txtLog.AppendText("Disconnesso" & Environment.NewLine)
    End Sub


    Private Sub btnSendAll_Click(sender As Object, e As EventArgs) Handles btnSendAll.Click
        Dim comando = generateCommand()
        inviaComando(comando)
    End Sub

    Private Function stringToCommad(s As String) As Comando
        Dim comando As New Comando
        Dim subs As String()
        s = s.Trim()
        If s.Contains("!") Then
            s = s.Replace("!", "")
            subs = s.Split(",")
            type = subs(0)
            comando.setCommand(type)
            If (type = "M1" Or type = "M2" Or type = "M3" Or type = "M4" Or type = "M5") Then
                comando.setPoint(subs(1), subs(2))
                comando.setSpeed(subs(3))
            ElseIf (Type = "M6") Then
                comando.setZAxisCommand(subs(1))
            ElseIf type = "M7" Then
                comando.setGripper(CInt(subs(1)))
            ElseIf (Type = "C1") Then
                comando.setDelay(subs(1))
            ElseIf type = "S4" Then
                comando.setAccel(subs(0))
            End If
            Return comando
        Else
                MsgBox("ERRORE, MANCA IL !")
        End If
    End Function
    Private Sub inviaComando(comando As Comando)
        Dim risp As String
        Dim Buffer() As Byte
        tmrData.Stop()
        If Not String.IsNullOrEmpty(comando.getCommandType) Then
            Buffer = UTF8.GetBytes(comando.getCommandType + comando.getValue + endCommand)
            txtLog.AppendText("Inviato: " & comando.getCommandType + comando.getValue + endCommand + Environment.NewLine())
            NetStream.Write(Buffer, 0, Buffer.Length)
            risp = controllo().Trim
            If Not risp = okMessage Then
                state = errorState
                MsgBox("ERRORE DI COMUNICAZIONE CON IL SERVER!")
                Exit Sub
            End If
            txtLog.AppendText("Ricevuto: ")
            txtLog.AppendText(risp)
            txtLog.AppendText(Environment.NewLine)
            If comando.isMovement Then
                currentCommand = comando
            End If
        End If
            tmrData.Start()

    End Sub

    Private Function loadFromFile(path As String) As List(Of Comando)
        Dim fileReader As String
        Dim l As New List(Of Comando)
        fileReader = My.Computer.FileSystem.ReadAllText(path)
        For Each n As String In fileReader.Split(Environment.NewLine)
            l.Add(stringToCommad(n))
        Next
        Return l
    End Function
    Private Function controllo() As String
        Dim risp As String = ""
        Dim t = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds()
        While risp = ""
            If DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() - t > timeout Then
                Return "ERR"
            End If
            If Client.Available > 0 Then
                'Li legge dallo stream
                Dim Buffer_(Client.Available - 1) As Byte
                NetStream.Read(Buffer_, 0, Buffer_.Length)
                'Li trasforma in una stringa
                risp = UTF8.GetString(Buffer_)
            End If
        End While
        Return risp
    End Function

    Private Function generateCommand() As Comando
        setAll()
        Return New Comando(type, XVal, YVal, speed, acceleration, zCommand, gripper, delay)

    End Function

    Private Sub setAll()
        If comboComando.Text IsNot "" Then
            type = comboComando.Text
        End If
        If IsNumeric((txtX.Text)) Then
            XVal = txtX.Text
        End If

        If IsNumeric((txtY.Text)) Then
            YVal = txtY.Text
        End If

        If IsNumeric((txtSpeed.Text)) Then
            speed = txtSpeed.Text
        End If

        If IsNumeric((txtZCode.Text)) Then
            zCommand = txtZCode.Text
        End If

        If chkGripper.Checked Then
            gripper = 1
        Else
            gripper = 0
        End If

        If IsNumeric((txtDelay.Text)) Then
            delay = txtDelay.Text
        End If

        If IsNumeric(txtAccel.Text) Then
            acceleration = txtAccel.Text
        End If
    End Sub

    Private Sub comboComando_DropDownClosed(sender As Object, e As EventArgs) Handles comboComando.TextChanged
        If (comboComando.Text = "M1" Or comboComando.Text = "M2" Or comboComando.Text = "M3" Or comboComando.Text = "M4") Then
            txtX.Enabled = True
            txtY.Enabled = True
            txtSpeed.Enabled = True
            txtZCode.Enabled = False
            chkGripper.Enabled = False
            txtDelay.Enabled = False
            txtAccel.Enabled = False


        ElseIf (comboComando.Text = "M5") Then
            txtX.Enabled = True
            txtY.Enabled = True
            txtSpeed.Enabled = True
            txtZCode.Enabled = False
            chkGripper.Enabled = False
            txtDelay.Enabled = False
            txtAccel.Enabled = False



        ElseIf (comboComando.Text = "M6") Then
            txtX.Enabled = False
            txtY.Enabled = False
            txtSpeed.Enabled = False
            txtZCode.Enabled = True
            chkGripper.Enabled = False
            txtDelay.Enabled = False
            txtAccel.Enabled = False


        ElseIf (comboComando.Text = "M7") Then
            txtX.Enabled = False
            txtY.Enabled = False
            txtSpeed.Enabled = False
            txtZCode.Enabled = False
            chkGripper.Enabled = True
            txtDelay.Enabled = False
            txtAccel.Enabled = False


        ElseIf (comboComando.Text = "M8") Then
            txtX.Enabled = False
            txtY.Enabled = False
            txtSpeed.Enabled = False
            txtZCode.Enabled = False
            chkGripper.Enabled = False
            txtDelay.Enabled = False
            txtAccel.Enabled = False

        ElseIf (comboComando.Text = "C1") Then
            txtX.Enabled = False
            txtY.Enabled = False
            txtSpeed.Enabled = False
            txtZCode.Enabled = False
            chkGripper.Enabled = False
            txtDelay.Enabled = True
            txtAccel.Enabled = False

        ElseIf (comboComando.Text = "S4") Then
            txtX.Enabled = False
            txtY.Enabled = False
            txtSpeed.Enabled = False
            txtZCode.Enabled = False
            chkGripper.Enabled = False
            txtDelay.Enabled = False
            txtAccel.Enabled = True

        End If
    End Sub

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        txtIP.Text = defaultIP
        btnSendAll.Enabled = False
        cmdYnegative.Enabled = False
        cmdYpositive.Enabled = False
        cmdXnegative.Enabled = False
        cmdXpositive.Enabled = False

    End Sub

    Private Sub cmdAdd_Click(sender As Object, e As EventArgs) Handles cmdAdd.Click
        setAll()
        listComandi.Add(generateCommand)
        lvComandi.Items.Clear()
        For Each c As Comando In listComandi
            lvComandi.Items.Add(c.getCommandType & c.getValue & "!")
        Next
    End Sub

    Private Sub cmdCiclo_Click(sender As Object, e As EventArgs) Handles cmdCiclo.Click
        If listComandi.Count > 1 Then
            startCiclo = Not startCiclo

            If startCiclo Then
                cmdCiclo.Text = "STOP CICLO"
                ciclo.inserisciLista(listComandi)
                inviaComando(ciclo.getNext)
                'For Each c As Comando In listComandi
                '    inviaComando(c)
                '    Threading.Thread.Sleep(10)
                'Next
            Else
                cmdCiclo.Text = "AVVIA CICLO"

            End If
        End If

    End Sub

    Private Sub cmdClearList_Click(sender As Object, e As EventArgs) Handles cmdClearList.Click
        startCiclo = False
        lvComandi.Items.Clear()
        listComandi.Clear()
    End Sub

    Private Sub cmdEmr_Click(sender As Object, e As EventArgs) Handles cmdEmr.Click
        inviaComando(New Comando(emrCommand))
    End Sub

    Private Sub cmdReset_Click(sender As Object, e As EventArgs) Handles cmdReset.Click
        inviaComando(New Comando(resetCommand))
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Dim fd As New OpenFileDialog
        Dim path As String
        If fd.ShowDialog() = DialogResult.OK Then
            path = fd.FileName
        End If
        listComandi = loadFromFile(path)
        lvComandi.Items.Clear()
        For Each c As Comando In listComandi
            lvComandi.Items.Add(c.getCommandType & c.getValue & "!")
        Next

    End Sub

    Private Sub chkManual_CheckedChanged(sender As Object, e As EventArgs) Handles chkManual.CheckedChanged
        If chkManual.Checked Then
            cmdYnegative.Enabled = True
            cmdYpositive.Enabled = True
            cmdXnegative.Enabled = True
            cmdXpositive.Enabled = True
        Else
            cmdYnegative.Enabled = False
            cmdYpositive.Enabled = False
            cmdXnegative.Enabled = False
            cmdXpositive.Enabled = False
        End If
    End Sub

    Private Sub cmdYpositive_Click(sender As Object, e As EventArgs) Handles cmdYpositive.Click
        currentCommand.setY(currentCommand.getY + numStepManuale.Value)
        inviaComando(currentCommand)
    End Sub

    Private Sub cmdYnegative_Click(sender As Object, e As EventArgs) Handles cmdYnegative.Click
        currentCommand.setY(currentCommand.getY - numStepManuale.Value)
        inviaComando(currentCommand)
    End Sub

    Private Sub cmdXpositive_Click(sender As Object, e As EventArgs) Handles cmdXpositive.Click
        currentCommand.setX(currentCommand.getX + numStepManuale.Value)
        inviaComando(currentCommand)
    End Sub

    Private Sub cmdXnegative_Click(sender As Object, e As EventArgs) Handles cmdXnegative.Click
        currentCommand.setX(currentCommand.getX - numStepManuale.Value)
        inviaComando(currentCommand)
    End Sub
End Class