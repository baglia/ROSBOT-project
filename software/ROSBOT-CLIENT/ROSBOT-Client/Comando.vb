Public Class Comando
    Private commandType As String
    Private nextPoint As New PointF
    Private speed As String
    Private acceleration As String
    Private gripper As Integer
    Private zAxisCommand As String
    Private inputNumber As String
    Private waitTime As String

    Public Sub New()
        commandType = ""
        nextPoint.X = 0
        nextPoint.Y = 0
        speed = "0"
        acceleration = "0"
        gripper = 0
        zAxisCommand = "0"
        inputNumber = "0"
        waitTime = "0"
    End Sub

    Public Sub New(_commandType As String, _valX As String, _valY As String, _speed As String, _accel As String, _zAxisCommand As String, _gripper As Integer, _waitTime As String)
        commandType = _commandType
        nextPoint.X = _valX
        nextPoint.Y = _valY
        speed = _speed
        acceleration = _accel
        gripper = _gripper
        zAxisCommand = _zAxisCommand
        inputNumber = "0"
        waitTime = _waitTime
    End Sub

    Public Sub New(_commandType As String, _valX As String, _valY As String, _speed As String)
        commandType = _commandType
        nextPoint.X = _valX
        nextPoint.Y = _valY
        speed = _speed
        acceleration = "0"
        gripper = 0
        zAxisCommand = "0"
        inputNumber = "0"
        waitTime = "0"
    End Sub
    Public Sub New(_commadTyoe As String, _gripper As Integer)
        commandType = _commadTyoe
        nextPoint.X = 0
        nextPoint.Y = 0
        speed = "0"
        acceleration = "0"
        gripper = _gripper
        zAxisCommand = "0"
        inputNumber = "0"
        waitTime = "0"
    End Sub

    Public Sub New(_commadTyoe As String)
        commandType = _commadTyoe
        nextPoint.X = 0
        nextPoint.Y = 0
        speed = "0"
        acceleration = "0"
        gripper = 0
        zAxisCommand = "0"
        inputNumber = "0"
        waitTime = "0"
    End Sub

    Public Sub New(_commadTyoe As String, _zAxisCommand As String)
        commandType = _commadTyoe
        nextPoint.X = 0
        nextPoint.Y = 0
        speed = "0"
        acceleration = "0"
        gripper = 0
        zAxisCommand = _zAxisCommand
        inputNumber = "0"
        waitTime = "0"
    End Sub


    Public Function isMovement() As Boolean
        If (commandType = "M1" Or commandType = "M2" Or commandType = "M3" Or commandType = "M4" Or commandType = "M5") Then
            Return True
        Else
            Return False
        End If
    End Function
    Public Sub setPoint(_valX As String, _valY As String)
        nextPoint.X = _valX
        nextPoint.Y = _valY
    End Sub
    Public Sub setX(_x As Integer)
        nextPoint.X = _x
    End Sub
    Public Sub setY(_Y As Integer)
        nextPoint.Y = _Y
    End Sub

    Public Function getX() As Integer
        Return nextPoint.X
    End Function

    Public Function getY() As Integer
        Return nextPoint.Y
    End Function

    Public Sub setCommand(_commandType As String)
        commandType = _commandType
    End Sub

    Public Sub setSpeed(_speed As String)
        speed = _speed
    End Sub

    Public Sub setGripper(_gripper As Integer)
        gripper = _gripper
    End Sub

    Public Sub setZAxisCommand(_zAxisCommand As String)
        zAxisCommand = _zAxisCommand
    End Sub
    Public Sub setDelay(_delay As String)
        waitTime = _delay
    End Sub

    Public Sub setAccel(_accel As String)
        acceleration = _accel
    End Sub
    Public Function getValue() As String
        Dim value As String = ""
        If (commandType = "M1" Or commandType = "M2" Or commandType = "M3" Or commandType = "M4" Or commandType = "M5") Then
            value = "|" + nextPoint.X.ToString + "," + nextPoint.Y.ToString + "," + "1" + "," + speed

        ElseIf (commandType = "M6") Then
            value = "|" + zAxisCommand

        ElseIf (commandType = "M7") Then
            value = "|" + gripper.ToString

        ElseIf (commandType = "M8" Or commandType = "S2" Or commandType = "S3") Then
            value = ""

        ElseIf (commandType = "C1") Then
            value = "|" + waitTime
        ElseIf (commandType = "S4") Then
            value = "|" + acceleration
        End If

        Return value
    End Function

    Public Function getCommandType() As String
        Return commandType
    End Function


End Class
