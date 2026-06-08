В работе использована библиотека SFML.

Чтобы проект собрался на windows, необходимо, чтобы SFML был в папке 'C:\SFML',
а также заменить текст файла tasks.json на следующий:

{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build SFML app",
            "type": "shell",
            "command": "g++",
            "args": [
                "-std=c++17",
                "${workspaceFolder}/*.cpp",
                "-o",
                "${workspaceFolder}/app.exe",
                "-IC:/SFML/include",
                "-LC:/SFML/lib",
                "-lsfml-graphics",
                "-lsfml-window",
                "-lsfml-system"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ]
        }
    ]
}

Я не проверял, но вроде должно сработать. Для запуска на mac ничего менять не нужно,
при условии, что библиотека SFML установленна.