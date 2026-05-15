"""
Lyoptimus Sim-Adv Server
Advanced simulator with tray and protective foil parameters.
"""

import os
import subprocess
import tempfile
import sqlite3
import json
from datetime import datetime
from flask import Flask, request, jsonify, send_from_directory, session
from flask_cors import CORS
import pytz
from users import USERS, ADMIN_USERNAME, ADMIN_PASSWORD, CUSTOMERS

app = Flask(__name__)
app.secret_key = 'lyoptimus_sim_adv_secret_2025'
CORS(app, supports_credentials=True)

BASE_DIR      = os.path.dirname(os.path.abspath(__file__))
TEMPLATES     = os.path.join(BASE_DIR, 'templates')
STATIC        = os.path.join(BASE_DIR, 'static')
CUSTOMERS_DIR = os.path.join(BASE_DIR, 'customers')

EXECUTABLE_NAME = 'kv_1d_dried_layer_adv'
EXECUTABLE_PATH = os.path.join(BASE_DIR, EXECUTABLE_NAME)

# ── DATABASE ──────────────────────────────────────────────────────────────────
def get_db(customer_id):
    db_path = os.path.join(CUSTOMERS_DIR, customer_id, 'simulations.db')
    conn = sqlite3.connect(db_path)
    conn.row_factory = sqlite3.Row
    conn.execute('''CREATE TABLE IF NOT EXISTS simulations (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        timestamp TEXT NOT NULL,
        username TEXT NOT NULL,
        shelf_temp REAL,
        chamber_pressure REAL,
        drying_time_h REAL,
        final_tb REAL,
        kv_total REAL
    )''')
    conn.commit()
    return conn

def save_simulation(customer_id, username, params, results):
    try:
        ce = results.get('cycle_end', {})
        conn = get_db(customer_id)
        conn.execute('''INSERT INTO simulations
            (timestamp, username, shelf_temp, chamber_pressure, drying_time_h, final_tb, kv_total)
            VALUES (?, ?, ?, ?, ?, ?, ?)''', (
            datetime.now(pytz.timezone('Europe/Madrid')).strftime('%Y-%m-%d %H:%M:%S'),
            username,
            params.get('shelf_temp'), params.get('chamber_pressure'),
            ce.get('time_h'), ce.get('tb_final'), ce.get('kv_total')
        ))
        conn.commit()
        conn.close()
    except Exception as e:
        print(f'DB save error: {e}')

# ── INPUT FILE BUILDER ────────────────────────────────────────────────────────
def build_input_file(params):
    p = params
    dosing_mode = int(p.get('dosing_mode', 0))
    lines = [
        "//Inlet data for Kv_1d_dried_layer, do not change/modify lines with \"//\" at the begining",
        "//Experimental testing data",
        "//Dosing in mass[kg] (0) of volume [l] (1)",
        str(dosing_mode),
        "//Initial filling weight [kg] (only if dosing is in mass) 0.001132",
        str(p.get('initial_fill_weight', 0.030)),
        "//Fraction of solute",
        str(p.get('fraction_solute', 0.05)),
        "//Final filling weight [kg] (only if dosing is in mass)",
        str(p.get('final_fill_weight', 0.0015)),
        "//Initial filling volume [l] (only if dosing is in volume)",
        str(p.get('initial_fill_volume', 0.02357)),
        "//Final filling volume [l] (only if dosing is in volume)",
        str(p.get('final_fill_volume', 0)),
        "//Vial data",
        "//Quantity of vials",
        str(int(p.get('qty_vials', 10))),
        "//vials internal diameter [m]",
        str(p.get('vial_id', 0.020)),
        "//vials external diameter [m]",
        str(p.get('vial_od', 0.022)),
        "//vial heat transfer parameter 1 [W/m2K] 11.51",
        str(p.get('kv1', 11.51)),
        "//vial heat transfer parameter 2 [W/m2KPa] 0.28",
        str(p.get('kv2', 0.28)),
        "//vial heat transfer paramenter 3 [1/Pa] 0.00345",
        str(p.get('kv3', 0.00345)),
        "//Stopper data",
        "//Stopper resistance coeficient 1 [kg/s Pa]",
        str(p.get('stopper_r1', 0.00000001)),
        "//Stopper resistance coeficient 2 [kg/s Pa^2]",
        str(p.get('stopper_r2', 0.00000000264)),
        "//Tray data",
        "//tray heat transfer parameter 1 [W/m2K] 1.1",
        str(p.get('tray_c1', 25)),
        "//tray heat transfer parameter 2 [W/m2KPa] 0.7",
        str(p.get('tray_c2', 0)),
        "//tray heat transfer paramenter 3 [1/Pa] 0.01",
        str(p.get('tray_c3', 0)),
        "//Protective foil data",
        "//Protective foil resistance 1 (inner) [m/s]80153.1864, 671942.88",
        str(p.get('foil1_rm', 13000)),
        "//Protective foil area 1 (inner) [m2]0.00484",
        str(p.get('foil1_am', 20)),
        "//Protective foil (inner) heat transfer parameter 1 [W/m2K](30% reduction)40.81",
        str(p.get('foil1_c1', 1e20)),
        "//Protective foil (inner)heat transfer parameter 2 [W/m2KPa]",
        str(p.get('foil1_c2', 0)),
        "//Protective foil (inner) heat transfer paramenter 3 [1/Pa]",
        str(p.get('foil1_c3', 0)),
        "//Protective foil resistance 2 (outer) [m/s]80153.1864, 671942.88",
        str(p.get('foil2_rm', 0)),
        "//Protective foil area 2 (outer) [m2]0.00484",
        str(p.get('foil2_am', 20)),
        "//Protective foil (outer) heat transfer parameter 1 [W/m2K] (30% reduction)40.81",
        str(p.get('foil2_c1', 100)),
        "//Protective foil (outer)heat transfer parameter 2 [W/m2KPa]",
        str(p.get('foil2_c2', 0)),
        "//Protective foil (outer) heat transfer paramenter 3 [1/Pa]",
        str(p.get('foil2_c3', 0)),
        "//Product data",
        "//R0[m/s] (product resistance coeficient 1)10000",
        str(p.get('r0', 10000)),
        "//A1[1/s] (product resistance coeficient 2)43400000",
        str(p.get('a1', 43400000)),
        "//A2[1/m] (product resistance coeficient 3)160",
        str(p.get('a2', 160)),
        "//Freeze dryer settings",
        "//Shelves fluid temperature [ºC]",
        str(p.get('shelf_temp', -30)),
        "//Chamber pressure [mbar]",
        str(p.get('chamber_pressure', 0.05)),
        "//Constant thermophisical properties",
        "//Water density in solid state [kg/m3]",
        str(p.get('ice_density', 920)),
        "//ice conductivity [W/Km]",
        str(p.get('ice_conductivity', 2.5)),
        "//Water density in liquid state [kg/m3]",
        str(p.get('water_density', 997)),
        "//Numerical parameters",
        "//Number of nodes at frozen product height",
        str(int(p.get('num_nodes', 10000000))),
        "//Difference between guessed and calculated Pv [mbar]",
        str(p.get('pv_tolerance', 0.00001)),
        "//Output file parameters",
        "//Height step to print results[m]",
        str(p.get('height_step', 0.001)),
    ]
    return '\n'.join(lines) + '\n'

# ── OUTPUT PARSER ─────────────────────────────────────────────────────────────
def parse_output_file(content):
    lines = content.splitlines()
    result = {'first_calculations': {}, 'profile': [], 'cycle_end': {}}
    in_first_calc = False
    in_results = False
    in_cycle_end = False

    for line in lines:
        line = line.strip()
        if line == 'First calculations':
            in_first_calc = True; in_results = False; continue
        if line.startswith('Results'):
            in_first_calc = False; in_results = True; continue
        if line == 'Cycle end':
            in_results = False; in_cycle_end = True; continue

        if in_first_calc and ';' in line:
            parts = line.split(';')
            try: val = float(parts[1].strip())
            except: continue
            key = parts[0].strip()
            if 'Total heat transfer area' in key: result['first_calculations']['total_heat_area'] = val
            elif 'Total sublimation area' in key: result['first_calculations']['total_subl_area'] = val
            elif 'Total protective foil area 1' in key: result['first_calculations']['foil1_area'] = val
            elif 'Total protective foil area 2' in key: result['first_calculations']['foil2_area'] = val
            elif 'Initial frozen height' in key: result['first_calculations']['initial_frozen_height'] = val
            elif 'Final frozen height' in key: result['first_calculations']['final_frozen_height'] = val

        if in_results and ';' in line:
            if line.startswith('Fh'): continue
            # Skip separator lines
            if line.startswith(';') or line == ' ;': continue
            parts = [p.strip() for p in line.split(';')]
            try:
                row = {
                    'fh':    float(parts[0]),
                    'fd':    float(parts[1]),
                    'tt':    float(parts[2]),
                    'dtr':   float(parts[3]),
                    'pm2':   float(parts[4]),
                    'pm':    float(parts[5]),
                    'ps':    float(parts[6]),
                    'pv':    float(parts[7]),
                    'tv':    float(parts[8]),
                    'tb':    float(parts[9]),
                    'sbr':   float(parts[10]),
                    'dpm2':  float(parts[12]) if len(parts) > 12 else 0,
                    'dpm':   float(parts[13]) if len(parts) > 13 else 0,
                    'dps':   float(parts[14]) if len(parts) > 14 else 0,
                    'dpp':   float(parts[15]) if len(parts) > 15 else 0,
                    'dptot': float(parts[17]) if len(parts) > 17 else 0,
                }
                result['profile'].append(row)
            except: continue

        if in_cycle_end and line and ';' in line:
            parts = line.split(';')
            key = parts[0].strip()
            try: val = float(parts[1].strip().split()[0])
            except: continue
            if 'Time[h]' in key: result['cycle_end']['time_h'] = val
            elif 'Tb' in key: result['cycle_end']['tb_final'] = val
            elif 'Total heat transfer coeficient' in key: result['cycle_end']['kv_total'] = val
            elif 'Heat transfer coeficient vial' in key: result['cycle_end']['kv_vial'] = val
            elif 'Heat transfer coeficient tray' in key: result['cycle_end']['kv_tray'] = val
            elif 'Heat transfer coeficient foil 1' in key: result['cycle_end']['kv_foil1'] = val
            elif 'Heat transfer coeficient foil 2' in key: result['cycle_end']['kv_foil2'] = val

    return result

# ── AUTH ROUTES ───────────────────────────────────────────────────────────────
@app.route('/login')
def login_page():
    return send_from_directory(TEMPLATES, 'login.html')

@app.route('/api/login', methods=['POST'])
def api_login():
    data = request.get_json()
    username = data.get('username', '').strip()
    password = data.get('password', '')
    if username == ADMIN_USERNAME and password == ADMIN_PASSWORD:
        session['username'] = username
        session['role'] = 'admin'
        return jsonify({'success': True, 'redirect': '/admin'})
    if username in USERS and USERS[username]['password'] == password:
        session['username'] = username
        session['role'] = 'user'
        session['customer'] = USERS[username]['customer']
        session['name'] = USERS[username]['name']
        return jsonify({'success': True, 'redirect': f"/sim/{USERS[username]['customer']}"})
    return jsonify({'success': False}), 401

@app.route('/api/logout')
def api_logout():
    session.clear()
    return app.redirect('/login')

@app.route('/api/session')
def api_session():
    if 'username' not in session:
        return jsonify({'error': 'Not logged in'}), 401
    customer_id = session.get('customer', '')
    customer = CUSTOMERS.get(customer_id, {})
    return jsonify({
        'username': session['username'],
        'name': session.get('name', session['username']),
        'role': session.get('role'),
        'customer': customer_id,
        'customer_name': customer.get('name', ''),
        'customer_logo': customer.get('logo', '/static/logo.jpg')
    })

# ── SIMULATOR ROUTES ──────────────────────────────────────────────────────────
@app.route('/sim/<customer_id>')
def customer_sim(customer_id):
    if 'username' not in session:
        return app.redirect('/login')
    if session.get('role') == 'user' and session.get('customer') != customer_id:
        return app.redirect('/login')
    sim_path = os.path.join(CUSTOMERS_DIR, customer_id, 'simulator.html')
    if not os.path.exists(sim_path):
        return 'Customer not found', 404
    return send_from_directory(os.path.join(CUSTOMERS_DIR, customer_id), 'simulator.html')

@app.route('/customers/<customer_id>/customer_logo.jpg')
def customer_logo(customer_id):
    return send_from_directory(os.path.join(CUSTOMERS_DIR, customer_id), 'customer_logo.jpg')

# ── ADMIN ROUTES ──────────────────────────────────────────────────────────────
@app.route('/admin')
def admin_page():
    if session.get('role') != 'admin':
        return app.redirect('/login')
    return send_from_directory(TEMPLATES, 'admin.html')

@app.route('/api/admin/simulations')
def admin_simulations():
    if session.get('role') != 'admin':
        return jsonify({'error': 'Unauthorized'}), 401
    all_sims = []
    for customer_id in CUSTOMERS:
        try:
            conn = get_db(customer_id)
            rows = conn.execute('SELECT * FROM simulations ORDER BY timestamp DESC').fetchall()
            conn.close()
            for r in rows:
                d = dict(r); d['customer'] = customer_id
                all_sims.append(d)
        except: pass
    all_sims.sort(key=lambda x: x['timestamp'], reverse=True)
    return jsonify({'simulations': all_sims})

@app.route('/api/my-simulations')
def my_simulations():
    if 'username' not in session:
        return jsonify({'error': 'Unauthorized'}), 401
    customer_id = session.get('customer')
    if session.get('role') == 'admin' and request.args.get('customer'):
        customer_id = request.args.get('customer')
    if not customer_id:
        return jsonify({'simulations': []})
    try:
        conn = get_db(customer_id)
        if session.get('role') == 'admin':
            rows = conn.execute('SELECT * FROM simulations ORDER BY timestamp DESC').fetchall()
        else:
            rows = conn.execute('SELECT * FROM simulations WHERE username = ? ORDER BY timestamp DESC',
                                (session['username'],)).fetchall()
        conn.close()
        return jsonify({'simulations': [dict(r) for r in rows]})
    except Exception as e:
        return jsonify({'simulations': [], 'error': str(e)})

# ── CALCULATION ───────────────────────────────────────────────────────────────
@app.route('/api/calculate', methods=['POST'])
def calculate():
    if 'username' not in session:
        return jsonify({'error': 'Not logged in'}), 401
    if not os.path.exists(EXECUTABLE_PATH):
        return jsonify({'error': f'Executable not found: {EXECUTABLE_PATH}'}), 500

    import stat
    os.chmod(EXECUTABLE_PATH, os.stat(EXECUTABLE_PATH).st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

    params = request.get_json()
    if not params:
        return jsonify({'error': 'No input data received.'}), 400

    with tempfile.TemporaryDirectory() as tmpdir:
        input_path  = os.path.join(tmpdir, 'input.dat')
        output_path = os.path.join(tmpdir, 'results.dat')
        with open(input_path, 'w') as f:
            f.write(build_input_file(params))
        try:
            proc = subprocess.run([EXECUTABLE_PATH, input_path, output_path],
                                  capture_output=True, text=True, timeout=120)
        except subprocess.TimeoutExpired:
            return jsonify({'error': 'Calculation timed out.'}), 500
        except Exception as e:
            return jsonify({'error': f'Failed to run executable: {str(e)}'}), 500

        if proc.returncode != 0:
            return jsonify({'error': f'Executable returned an error: {proc.stderr or proc.stdout}'}), 500
        if not os.path.exists(output_path):
            return jsonify({'error': 'Output file not generated.'}), 500
        with open(output_path, 'r', errors='replace') as f:
            output_content = f.read()

    try:
        parsed = parse_output_file(output_content)
    except Exception as e:
        return jsonify({'error': f'Failed to parse output: {str(e)}'}), 500

    if session.get('role') == 'user':
        save_simulation(session['customer'], session['username'], params, parsed)

    return jsonify(parsed)

# ── STATIC ────────────────────────────────────────────────────────────────────
@app.route('/static/<path:filename>')
def static_files(filename):
    return send_from_directory(STATIC, filename)

@app.route('/api/health')
def health():
    return jsonify({
        'status': 'ok',
        'service': 'lyoptimus-sim-adv',
        'executable_found': os.path.exists(EXECUTABLE_PATH)
    })

@app.route('/api/debug')
def debug():
    customers_contents = {}
    if os.path.exists(CUSTOMERS_DIR):
        for item in os.listdir(CUSTOMERS_DIR):
            item_path = os.path.join(CUSTOMERS_DIR, item)
            if os.path.isdir(item_path):
                customers_contents[item] = os.listdir(item_path)
    return jsonify({
        'base_dir': BASE_DIR,
        'customers_dir': CUSTOMERS_DIR,
        'customers_exists': os.path.exists(CUSTOMERS_DIR),
        'customers_contents': customers_contents,
        'templates_dir': TEMPLATES,
        'templates_exists': os.path.exists(TEMPLATES),
        'templates_contents': os.listdir(TEMPLATES) if os.path.exists(TEMPLATES) else [],
        'cwd': os.getcwd(),
        'listdir_cwd': os.listdir('.')
    })

if __name__ == '__main__':
    print("Lyoptimus Sim-Adv Server running on http://localhost:5002")
    app.run(debug=True, host='0.0.0.0', port=5002)
