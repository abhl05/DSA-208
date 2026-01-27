param(
    [int]$testNum,
    [string]$algorithm = "kruskals",
    [string]$program = "kruskal"
)

try {
    # Read files
    $outputFile = "$program" + "_out" + "$testNum.txt"
    $fullOutput = @(Get-Content $outputFile -ErrorAction Stop)
    $expectedOutput = @(Get-Content "sampleio\sampleio\$algorithm\out$testNum.txt" -ErrorAction Stop)

    # Extract MST cost (first line)
    $myCost = [string]$fullOutput[0]
    $expCost = [string]$expectedOutput[0]

    # Count edges (lines matching "n1 n2" pattern)
    $myEdges = @($fullOutput | Where-Object { $_ -match '^\d+ \d+$' }).Count
    $expEdges = @($expectedOutput | Where-Object { $_ -match '^\d+ \d+$' }).Count

    # Compare and output results
    if ($myCost -eq $expCost -and $myEdges -eq $expEdges) {
        Write-Host "Test $testNum : PASSED [Cost: $myCost | Edges: $myEdges]"
        exit 0
    } else {
        Write-Host "Test $testNum : FAILED"
        if ($myCost -ne $expCost) {
            Write-Host "  Cost mismatch - Expected: $expCost, Got: $myCost"
        }
        if ($myEdges -ne $expEdges) {
            Write-Host "  Edge count mismatch - Expected: $expEdges, Got: $myEdges"
        }
        exit 1
    }
}
catch {
    Write-Host "Test $testNum : ERROR - $_"
    exit 1
}

