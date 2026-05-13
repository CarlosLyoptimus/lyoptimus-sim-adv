"""
Lyoptimus Sim-Adv — User configuration file
To add/remove users, edit this file and redeploy.
"""

# ── ADMIN ─────────────────────────────────────────────────────────────────────
ADMIN_USERNAME = 'admin'
ADMIN_PASSWORD = 'lyoptimus2025'   # ← CHANGE THIS

# ── CUSTOMERS ─────────────────────────────────────────────────────────────────
CUSTOMERS = {
    'customer_a': {
        'name': 'Customer A',
        'logo': '/customers/customer_a/customer_logo.jpg'
    },
    'customer_b': {
        'name': 'Customer B',
        'logo': '/customers/customer_b/customer_logo.jpg'
    },
    'customer_c': {
        'name': 'Customer C',
        'logo': '/customers/customer_c/customer_logo.jpg'
    },
}

# ── USERS ─────────────────────────────────────────────────────────────────────
USERS = {
    # Customer A users
    'a_user1': {
        'password': 'passA1',
        'customer': 'customer_a',
        'name': 'User 1 (A)'
    },
    # Customer B users
    'b_user1': {
        'password': 'passB1',
        'customer': 'customer_b',
        'name': 'User 1 (B)'
    },
    # Customer C users
    'c_user1': {
        'password': 'passC1',
        'customer': 'customer_c',
        'name': 'User 1 (C)'
    },
}
