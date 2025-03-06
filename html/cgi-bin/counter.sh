#!/bin/bash
counter_file="/tmp/counter.txt"
if [[ -f "$counter_file" ]]; then
    greet_count=$(cat "$counter_file")
else
    greet_count=0
fi
greet_count=$((greet_count + 1))
echo "$greet_count" > "$counter_file"
query_string=$(echo "$QUERY_STRING" | tr '&' '\n')
name=""
for param in $query_string; do
    key=$(echo $param | cut -d= -f1)
    value=$(echo $param | cut -d= -f2-)
    if [[ "$key" == "name" ]]; then
        name=$value
    fi
done

if [ -z "$name" ]; then
    name="visitante"
fi
cat <<EOF
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Saludo Interactivo</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f8ff; padding: 20px; }
        h1 { color: #2e8b57; }
        p { font-size: 18px; }
        form { margin-top: 20px; }
        input[type="text"] { padding: 5px; font-size: 14px; }
        input[type="submit"] { padding: 5px 10px; font-size: 14px; background-color: #2e8b57; color: white; border: none; }
    </style>
</head>
<body>

<h1>¡Hola, $name!</h1>
<p>Este es el saludo número $greet_count</p>
<p>Por favor, ingresa tu nombre para recibir un saludo personalizado.</p>

<form method="GET" action="">
    <input type="text" name="name" placeholder="Tu nombre" value="$name" required>
    <input type="submit" value="Saludar">
</form>

</body>
</html>
EOF

