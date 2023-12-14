# arduino_panel/urls.py
from django.urls import path
from .views import index, send_command

urlpatterns = [
    path('', index, name='index'),
    path('send_command', send_command, name='send_command'),
]
