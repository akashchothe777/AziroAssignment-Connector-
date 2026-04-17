# AziroAssignment-Connector-
Connector project is for data backup from one drive to azure cloud

Here is the classdiagram,

<img width="700" height="443" alt="ClassDiagram" src="https://github.com/user-attachments/assets/723099db-1b1c-44c9-bf91-f887d80354c8" />

It supports downloading/copying files from source folder to out local download folder and uploading/copying files in data folder to destination folder.

Also it supports downloading files from one drive and uploading the downloaded files to Azure Blob storage.

User need to pass a JSON configuration file which should have below details,

{
  "download_folder": "D:\\Assessment\\Connector\\DownloadFolder",
  "metadata_file" : "D:\\Assessment\\Connector\\md\\metadata_store.json",
  "source_url" : "",
  "source_access_token" : "",
  "sas_url" : "",
  "retry_count": 3
}

Here, 
download_folder : local folder where downloaded files from source are stored temporarily
metadata_file : a local file where the metadata of files are stored
source_url : source from where the tool will download the files
source_access_token : access token for the source
destination_sas_url : Azure blob sas url where files will be uploaded
retry_count : number of times the download or uploaded operation will be performed if failed
