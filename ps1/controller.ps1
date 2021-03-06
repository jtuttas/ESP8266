﻿$global:connected=$false;
function update($values) {
    $progressBarRed.Value=$values.dimValueRed;
    $progressBarGreen.Value=$values.dimValueGreen;
    $progressBarBlue.Value=$values.dimValueBlue;
    $progressBarPressure.Value=$values.pressure
    $progressBarTemperature.Value=[int]$values.temperature
    $labelPressure.Text="Luftdruck = "+$values.pressure+" Pa"
    $labelTemperature.Text="Temperatur = "+$values.temperature+" °C";
}
#Generated Form Function
function GenerateForm {
########################################################################
# Code Generated By: SAPIEN Technologies PrimalForms (Community Edition) v1.0.10.0
# Generated On: 21.12.2016 17:17
# Generated By: Jörg
########################################################################

#region Import the Assemblies
[reflection.assembly]::loadwithpartialname("System.Drawing") | Out-Null
[reflection.assembly]::loadwithpartialname("System.Windows.Forms") | Out-Null
#endregion

#region Generated Form Objects
$form = New-Object System.Windows.Forms.Form
$groupBox2 = New-Object System.Windows.Forms.GroupBox
$progressBarPressure = New-Object System.Windows.Forms.ProgressBar
$labelPressure = New-Object System.Windows.Forms.Label
$labelTemperature = New-Object System.Windows.Forms.Label
$progressBarTemperature = New-Object System.Windows.Forms.ProgressBar
$statusBar = New-Object System.Windows.Forms.StatusBar
$groupBox1 = New-Object System.Windows.Forms.GroupBox
$labelPreview = New-Object System.Windows.Forms.Label
$buttonSet = New-Object System.Windows.Forms.Button
$progressBarBlue = New-Object System.Windows.Forms.ProgressBar
$progressBarGreen = New-Object System.Windows.Forms.ProgressBar
$progressBarRed = New-Object System.Windows.Forms.ProgressBar
$trackBarBlue = New-Object System.Windows.Forms.TrackBar
$trackBarGreen = New-Object System.Windows.Forms.TrackBar
$label6 = New-Object System.Windows.Forms.Label
$label5 = New-Object System.Windows.Forms.Label
$label4 = New-Object System.Windows.Forms.Label
$trackBarRed = New-Object System.Windows.Forms.TrackBar
$buttonConnect = New-Object System.Windows.Forms.Button
$label1 = New-Object System.Windows.Forms.Label
$textBoxController = New-Object System.Windows.Forms.TextBox
$timer1 = New-Object System.Windows.Forms.Timer
$InitialFormWindowState = New-Object System.Windows.Forms.FormWindowState
#endregion Generated Form Objects

#----------------------------------------------
#Generated Event Script Blocks
#----------------------------------------------
#Provide Custom Code for events specified in PrimalForms.

$handler_timer1_Tick= 
{
    Write-Host "tick"
    $values = Invoke-RestMethod $textBoxController.Text
    update $values;
}
$handler_trackBarRed_ValueChanged= 
{
    $r=$trackBarRed.Value/4;
    $g=$trackBarGreen.Value/4;
    $b=$trackBarBlue.Value/4;
    if ($r -gt 255) {$r=255;}
    if ($g -gt 255) {$g=255;}
    if ($b -gt 255) {$b=255;}
    $labelPreview.BackColor = [System.Drawing.Color]::FromArgb($r,$g,$b);
}

$handler_buttonSet_Click= 
{
 [String]$r=$trackBarRed.Value
 [String]$g=$trackBarGreen.Value
 [String]$b=$trackBarBlue.Value
 $r="0"*(4-$r.Length)+$r
 $g="0"*(4-$g.Length)+$g
 $b="0"*(4-$b.Length)+$b
 $values = Invoke-RestMethod $($textBoxController.Text+"/dim/r="+$r+"/g="+$g+"/b="+$b+"/")
 update $values;
}

$handler_buttonConnect_Click= 
{
    Write-Host "cick"$global:connected
    if ($global:connected) {
        $labelPressure.Enabled=$false
        $labelTemperature.Enabled=$false
        $buttonSet.Enabled=$false
        $buttonConnect.Text="Connect"
        $textBoxController.Enabled=$true
        $progressBarPressure.Value=$progressBarPressure.Minimum
        $progressBarTemperature.Value=$progressBarTemperature.Minimum
        $progressBarRed.Value=$progressBarRed.Minimum
        $progressBarGreen.Value=$progressBarGreen.Minimum
        $progressBarBlue.Value=$progressBarBlue.Minimum
        $trackBarRed.Value=1;
        $trackBarGreen.Value=1;
        $trackBarBlue.Value=1;
        $labelPreview.BackColor = [System.Drawing.Color]::FromArgb($trackBarRed.Value/4,$trackBarGreen.Value/4,$trackBarBlue.Value/4);
        $global:connected=$false;
        $labelPressure.Text="Luftdruck = ? Pa"
        $labelTemperature.Text="Temperatur = ? °C";
        $timer1.Enabled=$false;
        $statusBar.Text="Not Connected"
    }
    else {
        try {
            $values = Invoke-RestMethod $textBoxController.Text
            if ([bool]($values.PSobject.Properties.name -match "pressure")) {
                $labelPressure.Enabled=$true
                $labelTemperature.Enabled=$true
                $buttonSet.Enabled=$true
                $buttonConnect.Text="Disconnect"
                $textBoxController.Enabled=$False
                update $values;
                $trackBarRed.Value=$values.dimValueRed;
                $trackBarGreen.Value=$values.dimValueGreen;
                $trackBarBlue.Value=$values.dimValueBlue;
                $r=$trackBarRed.Value/4;
                $g=$trackBarGreen.Value/4;
                $b=$trackBarBlue.Value/4;
                if ($r -gt 255) {$r=255;}
                if ($g -gt 255) {$g=255;}
                if ($b -gt 255) {$b=255;}

                $labelPreview.BackColor = [System.Drawing.Color]::FromArgb($r,$g,$b);
                $global:connected=$true;
                $timer1.Enabled=$true;
                $statusBar.Text="Verbunden mit "+$textBoxController.Text
            }
            else {
                $statusBar.Text = "Empfangene Daten fehlerhaft!"
            }
        }
        catch {
            $statusBar.Text = "Failed to connect to "+$textBoxController.Text
        }
    }
}

$handler_trackBarBlue_ValueChanged= 
{
    $r=$trackBarRed.Value/4;
    $g=$trackBarGreen.Value/4;
    $b=$trackBarBlue.Value/4;
    if ($r -gt 255) {$r=255;}
    if ($g -gt 255) {$g=255;}
    if ($b -gt 255) {$b=255;}
    $labelPreview.BackColor = [System.Drawing.Color]::FromArgb($r,$g,$b);
}

$handler_trackBarGreen_ValueChanged= 
{
    $r=$trackBarRed.Value/4;
    $g=$trackBarGreen.Value/4;
    $b=$trackBarBlue.Value/4;
    if ($r -gt 255) {$r=255;}
    if ($g -gt 255) {$g=255;}
    if ($b -gt 255) {$b=255;}
    $labelPreview.BackColor = [System.Drawing.Color]::FromArgb($r,$g,$b);
}

$OnLoadForm_StateCorrection=
{#Correct the initial state of the form to prevent the .Net maximized form issue
	$form.WindowState = $InitialFormWindowState
}


$OnLoadForm_StateCorrection=
{#Correct the initial state of the form to prevent the .Net maximized form issue
	$form.WindowState = $InitialFormWindowState
}

#----------------------------------------------
#region Generated Form Code
$form.AutoScaleMode = 0
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 334
$System_Drawing_Size.Width = 559
$form.ClientSize = $System_Drawing_Size
$form.DataBindings.DefaultDataSourceUpdateMode = 0
$form.MaximizeBox = $False
$form.MinimizeBox = $False
$form.Name = "form"
$form.Text = "ESP8266"


$groupBox2.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 12
$System_Drawing_Point.Y = 211
$groupBox2.Location = $System_Drawing_Point
$groupBox2.Name = "groupBox2"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 87
$System_Drawing_Size.Width = 531
$groupBox2.Size = $System_Drawing_Size
$groupBox2.TabIndex = 9
$groupBox2.TabStop = $False
$groupBox2.Text = "BMP180"

$form.Controls.Add($groupBox2)
$progressBarPressure.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 167
$System_Drawing_Point.Y = 52
$progressBarPressure.Location = $System_Drawing_Point
$progressBarPressure.Maximum = 105800
$progressBarPressure.Minimum = 95000
$progressBarPressure.Name = "progressBarPressure"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 344
$progressBarPressure.Size = $System_Drawing_Size
$progressBarPressure.TabIndex = 6
$progressBarPressure.Value = 95000

$groupBox2.Controls.Add($progressBarPressure)

$labelPressure.DataBindings.DefaultDataSourceUpdateMode = 0
$labelPressure.Enabled = $False

$labelPressure.ImageAlign = 64
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 7
$System_Drawing_Point.Y = 52
$labelPressure.Location = $System_Drawing_Point
$labelPressure.Name = "labelPressure"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 141
$labelPressure.Size = $System_Drawing_Size
$labelPressure.TabIndex = 5
$labelPressure.Text = "Luftdruck = ? Pa"
$labelPressure.TextAlign = 64

$groupBox2.Controls.Add($labelPressure)

$labelTemperature.DataBindings.DefaultDataSourceUpdateMode = 0
$labelTemperature.Enabled = $False

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 6
$System_Drawing_Point.Y = 20
$labelTemperature.Location = $System_Drawing_Point
$labelTemperature.Name = "labelTemperature"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 142
$labelTemperature.Size = $System_Drawing_Size
$labelTemperature.TabIndex = 4
$labelTemperature.Text = "Temperatur = ? °C"
$labelTemperature.TextAlign = 64

$groupBox2.Controls.Add($labelTemperature)

$progressBarTemperature.Anchor = 0
$progressBarTemperature.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 167
$System_Drawing_Point.Y = 20
$progressBarTemperature.Location = $System_Drawing_Point
$progressBarTemperature.Maximum = 40
$progressBarTemperature.Name = "progressBarTemperature"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 24
$System_Drawing_Size.Width = 344
$progressBarTemperature.Size = $System_Drawing_Size
$progressBarTemperature.TabIndex = 3

$groupBox2.Controls.Add($progressBarTemperature)


$statusBar.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 0
$System_Drawing_Point.Y = 312
$statusBar.Location = $System_Drawing_Point
$statusBar.Name = "statusBar"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 22
$System_Drawing_Size.Width = 559
$statusBar.Size = $System_Drawing_Size
$statusBar.TabIndex = 8
$statusBar.Text = "Not Connected"

$form.Controls.Add($statusBar)


$groupBox1.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 12
$System_Drawing_Point.Y = 41
$groupBox1.Location = $System_Drawing_Point
$groupBox1.Name = "groupBox1"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 164
$System_Drawing_Size.Width = 531
$groupBox1.Size = $System_Drawing_Size
$groupBox1.TabIndex = 7
$groupBox1.TabStop = $False
$groupBox1.Text = "RGB LED"

$form.Controls.Add($groupBox1)
$labelPreview.BackColor = [System.Drawing.Color]::FromArgb(255,227,227,227)
$labelPreview.BorderStyle = 2
$labelPreview.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 249
$System_Drawing_Point.Y = 95
$labelPreview.Location = $System_Drawing_Point
$labelPreview.Name = "labelPreview"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 57
$System_Drawing_Size.Width = 151
$labelPreview.Size = $System_Drawing_Size
$labelPreview.TabIndex = 16
$labelPreview.Text = "preview"
$labelPreview.TextAlign = 32

$groupBox1.Controls.Add($labelPreview)


$buttonSet.DataBindings.DefaultDataSourceUpdateMode = 0
$buttonSet.Enabled = $False
$buttonSet.Font = New-Object System.Drawing.Font("Microsoft Sans Serif",15.75,0,3,0)

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 249
$System_Drawing_Point.Y = 19
$buttonSet.Location = $System_Drawing_Point
$buttonSet.Name = "buttonSet"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 59
$System_Drawing_Size.Width = 151
$buttonSet.Size = $System_Drawing_Size
$buttonSet.TabIndex = 15
$buttonSet.Text = "Set"
$buttonSet.UseVisualStyleBackColor = $True
$buttonSet.add_Click($handler_buttonSet_Click)

$groupBox1.Controls.Add($buttonSet)

$progressBarBlue.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 425
$System_Drawing_Point.Y = 117
$progressBarBlue.Location = $System_Drawing_Point
$progressBarBlue.Maximum = 1023
$progressBarBlue.Name = "progressBarBlue"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 100
$progressBarBlue.Size = $System_Drawing_Size
$progressBarBlue.TabIndex = 14

$groupBox1.Controls.Add($progressBarBlue)

$progressBarGreen.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 425
$System_Drawing_Point.Y = 65
$progressBarGreen.Location = $System_Drawing_Point
$progressBarGreen.Maximum = 1023
$progressBarGreen.Name = "progressBarGreen"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 100
$progressBarGreen.Size = $System_Drawing_Size
$progressBarGreen.TabIndex = 13

$groupBox1.Controls.Add($progressBarGreen)

$progressBarRed.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 425
$System_Drawing_Point.Y = 19
$progressBarRed.Location = $System_Drawing_Point
$progressBarRed.Maximum = 1023
$progressBarRed.Name = "progressBarRed"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 100
$progressBarRed.Size = $System_Drawing_Size
$progressBarRed.TabIndex = 12

$groupBox1.Controls.Add($progressBarRed)

$trackBarBlue.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 66
$System_Drawing_Point.Y = 117
$trackBarBlue.Location = $System_Drawing_Point
$trackBarBlue.Maximum = 1023
$trackBarBlue.Minimum = 0
$trackBarBlue.Name = "trackBarBlue"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 45
$System_Drawing_Size.Width = 167
$trackBarBlue.Size = $System_Drawing_Size
$trackBarBlue.TabIndex = 11
$trackBarBlue.Value = 1
$trackBarBlue.add_ValueChanged($handler_trackBarBlue_ValueChanged)

$groupBox1.Controls.Add($trackBarBlue)

$trackBarGreen.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 65
$System_Drawing_Point.Y = 65
$trackBarGreen.Location = $System_Drawing_Point
$trackBarGreen.Maximum = 1023
$trackBarGreen.Minimum = 0
$trackBarGreen.Name = "trackBarGreen"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 45
$System_Drawing_Size.Width = 168
$trackBarGreen.Size = $System_Drawing_Size
$trackBarGreen.TabIndex = 10
$trackBarGreen.Value = 1
$trackBarGreen.add_ValueChanged($handler_trackBarGreen_ValueChanged)

$groupBox1.Controls.Add($trackBarGreen)

$label6.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 19
$System_Drawing_Point.Y = 116
$label6.Location = $System_Drawing_Point
$label6.Name = "label6"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 40
$label6.Size = $System_Drawing_Size
$label6.TabIndex = 9
$label6.Text = "blue"
$label6.TextAlign = 64

$groupBox1.Controls.Add($label6)

$label5.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 19
$System_Drawing_Point.Y = 65
$label5.Location = $System_Drawing_Point
$label5.Name = "label5"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 40
$label5.Size = $System_Drawing_Size
$label5.TabIndex = 8
$label5.Text = "green"
$label5.TextAlign = 64

$groupBox1.Controls.Add($label5)

$label4.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 7
$System_Drawing_Point.Y = 19
$label4.Location = $System_Drawing_Point
$label4.Name = "label4"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 52
$label4.Size = $System_Drawing_Size
$label4.TabIndex = 7
$label4.Text = "red"
$label4.TextAlign = 64

$groupBox1.Controls.Add($label4)

$trackBarRed.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 65
$System_Drawing_Point.Y = 19
$trackBarRed.Location = $System_Drawing_Point
$trackBarRed.Maximum = 1023
$trackBarRed.Minimum = 0
$trackBarRed.Name = "trackBarRed"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 45
$System_Drawing_Size.Width = 168
$trackBarRed.Size = $System_Drawing_Size
$trackBarRed.TabIndex = 1
$trackBarRed.Value = 1
$trackBarRed.add_ValueChanged($handler_trackBarRed_ValueChanged)

$groupBox1.Controls.Add($trackBarRed)



$buttonConnect.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 468
$System_Drawing_Point.Y = 9
$buttonConnect.Location = $System_Drawing_Point
$buttonConnect.Name = "buttonConnect"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 75
$buttonConnect.Size = $System_Drawing_Size
$buttonConnect.TabIndex = 2
$buttonConnect.Text = "Connect"
$buttonConnect.UseVisualStyleBackColor = $True
$buttonConnect.add_Click($handler_buttonConnect_Click)

$form.Controls.Add($buttonConnect)

$label1.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 12
$System_Drawing_Point.Y = 9
$label1.Location = $System_Drawing_Point
$label1.Name = "label1"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 59
$label1.Size = $System_Drawing_Size
$label1.TabIndex = 1
$label1.Text = "Controller:"
$label1.TextAlign = 64

$form.Controls.Add($label1)

$textBoxController.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 77
$System_Drawing_Point.Y = 11
$textBoxController.Location = $System_Drawing_Point
$textBoxController.Name = "textBoxController"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 20
$System_Drawing_Size.Width = 385
$textBoxController.Size = $System_Drawing_Size
$textBoxController.TabIndex = 0
$textBoxController.Text = "http://service.joerg-tuttas.de:8266/json"

$form.Controls.Add($textBoxController)

$timer1.Interval = 1000
$timer1.add_Tick($handler_timer1_Tick)

$labelPreview.BackColor="#000000"
$progressBarGreen.ForeColor = [System.Drawing.Color]::FromArgb(0,255,0,0)
$progressBarRed.ForeColor = [System.Drawing.Color]::FromArgb(255,0,0,0)
$progressBarBlue.ForeColor = [System.Drawing.Color]::FromArgb(0,0,255,0)
$progressBarRed.MarqueeAnimationSpeed = 0
$progressBarGreen.MarqueeAnimationSpeed = 0
$progressBarBlue.MarqueeAnimationSpeed = 0
$timer1.Enabled=$False;
$form.FormBorderStyle = "FixedSingle";

#endregion Generated Form Code

#Save the initial state of the form
$InitialFormWindowState = $form.WindowState
#Init the OnLoad event to correct the initial state of the form
$form.add_Load($OnLoadForm_StateCorrection)
#Show the Form
$form.ShowDialog()| Out-Null

} #End Function

#Call the Function
GenerateForm
