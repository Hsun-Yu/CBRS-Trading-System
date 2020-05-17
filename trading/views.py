from django.shortcuts import render
from django.http import HttpResponse

# Create your views here.
def index(request):
    return render(request, 'trading/index.html')

def login(request):
    return render(request, 'trading/login.html')

def signup(request):
    return render(request, 'trading/signup.html')