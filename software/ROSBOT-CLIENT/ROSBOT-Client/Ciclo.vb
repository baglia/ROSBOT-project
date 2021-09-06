Public Class Ciclo
    Private comandi As List(Of Comando)
    Private stato As Boolean
    Private index As Integer

    Public Sub New()
        stato = True
        index = -1
    End Sub

    Public Sub New(_comandi As List(Of Comando))
        comandi = _comandi
        stato = True
        index = -1
    End Sub

    Public Sub addCommand(_comando As Comando)
        comandi.Add(_comando)
    End Sub

    Public Sub inserisciLista(_comandi As List(Of Comando))
        comandi = _comandi
        stato = True
        index = -1
    End Sub


    Public Function getNext() As Comando
        If stato Then
            index += 1
            If index > comandi.Count - 1 Then
                index = 0
            End If
            stato = False
            Return comandi(index)
        End If
        Return Nothing
    End Function

    Public Sub setStato(_stato As Boolean)
        stato = _stato
    End Sub

    Public Sub reset()
        index = 0
        stato = True
    End Sub

    Public Function getState() As Boolean
        Return stato
    End Function

    Public Function getIndex() As Integer
        Return index
    End Function

    Public Function getList() As List(Of Comando)
        Return comandi
    End Function
End Class
