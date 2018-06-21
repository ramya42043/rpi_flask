from flask import Flask, render_template, request
import subprocess
from subprocess import PIPE
import os
import re
import time
import json
import collections


testcases = {}
tc_file_path = ""
base_dir = os.getcwd()


for i in os.listdir(base_dir+"/Results/"):
    os.remove(base_dir+"/Results/"+i)

def get_files(ftype=None):
	global testcases
	global tc_file_path
        tc_file_path = os.path.join(base_dir, 'Testcases', ftype)
        tc_files = [i for i in os.listdir(tc_file_path) if i.endswith(".py")]
	
	tc_file_count = len(tc_files)

	for i in range(1, tc_file_count+1):
		testcases['Testcase'+str(i)] = tc_files[i-1]
	return testcases

app = Flask(__name__)

@app.route("/", methods=['GET'])
def return_home():
	return render_template("home.html")

@app.route("/audio", methods=['GET'])
def return_audio():
	testcases = get_files(ftype='audio') 
	return render_template("audio.html", tcases=testcases)



@app.route("/video", methods=['GET'])
def return_video():
        testcases = get_files(ftype='video') 
        return render_template("video.html", tcases=testcases)
@app.route("/camera", methods=['GET'])
def return_camera():
        testcases = get_files(ftype='camera') 
        return render_template("camera.html", tcases=testcases)


@app.route("/testcases", methods=['POST'])
def execute_cases():
        fresults = collections.OrderedDict() 
	items = request.form.getlist('Testcase')
        if request.form.get("audio"):
            fresults["AUDIO"] = "" 
        if request.form.get("video"):
            fresults["VIDEO"] = "" 

        process = subprocess.Popen("uname -a", stdout=subprocess.PIPE, shell=True)
        uname_out = process.communicate()[0]
        mat = re.search(r"Linux\s(.+?)\s", uname_out) 
        if mat:
            board_name = mat.group(1)
            f = open(base_dir+"/Results/"+board_name+"_results.txt", "a")

        #print "items:", items
	status = {} 
	for item in items:
		#print "testcase name:", testcases[item]
		item = str(item)
		os.chdir(tc_file_path)
		process = subprocess.Popen('python {0}'.format(testcases[item]), stdout=PIPE, shell=True)  
		status[item] = (process.communicate()[0]).strip()
                fresults[testcases[item]]=status[item] 
		os.chdir(base_dir)
	#print "status:", status
        #print "FRESULTS:", fresults
        f.write(json.dumps(fresults))
        f.write("\n")


	return render_template("status.html", status=status)

@app.route("/test_results", methods=['GET'])
def return_results():
        os.chdir(base_dir+"/Results")
        filenames = os.listdir(base_dir+"/Results")
        t_results = [] 
        for filename in filenames:
            print "filename:", filename
            with open(filename, "r") as f:
                json_objs = f.readlines()
                print "JSON_OBJS", json_objs 

        for json_obj in json_objs:
            t_results.append(json.loads(json_obj, object_pairs_hook=collections.OrderedDict))

        #print "t results:", t_results
        

        return render_template("test_results.html", t_results = t_results)

		
if __name__ == "__main__":
	app.run(debug=True)
