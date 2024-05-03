from pydrive.drive import GoogleDrive
from quickstart import gauth
import time


# Create GoogleDrive instance with authenticated GoogleAuth instance.
drive = GoogleDrive(gauth)

def uploadCsvs():
    #Update data1.csv
    file1 = drive.CreateFile({'id': '1U4VgYg8wXU5_WD9xOqKVf1Gl9_mSgk-i'})
    file1.SetContentFile('data1.csv')
    file1['title'] = 'data1.csv'
    file1.Upload()
    #Update data2.csv
    file2 = drive.CreateFile({'id': '1U5NtH8VyQ2iVNiQiMnADBIgjK4xHgupb'})
    file2.SetContentFile('data2.csv')
    file2['title'] = 'data2.csv'
    file2.Upload()
    #Update data3.csv
    file3 = drive.CreateFile({'id': '1x_fwR0LecPh7ZTxa2tEOa7O5LDIax4Qd'})
    file3.SetContentFile('data3.csv')
    file3['title'] = 'data3.csv'
    file3.Upload()
    #Update data2.csv
    file4 = drive.CreateFile({'id': '1dt5ndjWEJ0QAFwKOEEDtRhOHlc26SfET'})
    file4.SetContentFile('data4.csv')
    file4['title'] = 'data4.csv'
    file4.Upload()
while(True):
    uploadCsvs()
    time.sleep(30)