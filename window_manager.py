from flask import Blueprint, Response

window_manager_bp = Blueprint('window_manager', __name__)

js_code = """
let dragData = {
    dragging: false,
    offsetX: 0,
    offsetY: 0,
    targetId: null,
};

function startDrag(e, id) {
    dragData.dragging = true;
    dragData.targetId = id;
    const target = document.getElementById(id);
    dragData.offsetX = e.clientX - target.offsetLeft;
    dragData.offsetY = e.clientY - target.offsetTop;
    document.body.style.userSelect = 'none';
}

window.addEventListener('mousemove', (e) => {
    if (!dragData.dragging) return;
    const target = document.getElementById(dragData.targetId);
    let newX = e.clientX - dragData.offsetX;
    let newY = e.clientY - dragData.offsetY;
    // Keep window inside viewport
    newX = Math.max(0, Math.min(newX, window.innerWidth - target.offsetWidth));
    newY = Math.max(0, Math.min(newY, window.innerHeight - target.offsetHeight - 80)); // 80 for dock height + margin
    target.style.left = newX + 'px';
    target.style.top = newY + 'px';
});

window.addEventListener('mouseup', () => {
    dragData.dragging = false;
    document.body.style.userSelect = 'auto';
});

function closeWindow(id) {
    const win = document.getElementById(id);
    win.style.display = 'none';
}

function minimizeWindow(id) {
    const win = document.getElementById(id);
    win.style.display = 'none';
}

function maximizeWindow(id) {
    const win = document.getElementById(id);
    if (win.classList.contains('maximized')) {
        // Restore
        win.style.top = win.dataset.prevTop;
        win.style.left = win.dataset.prevLeft;
        win.style.width = '300px';
        win.style.height = 'auto';
        win.classList.remove('maximized');
    } else {
        // Maximize
        win.dataset.prevTop = win.style.top;
        win.dataset.prevLeft = win.style.left;
        win.style.top = '0px';
        win.style.left = '0px';
        win.style.width = '100vw';
        win.style.height = 'calc(100vh - 80px)';
        win.classList.add('maximized');
    }
}

function openWindow(id) {
    const win = document.getElementById(id);
    win.style.display = 'flex';
    win.style.zIndex = 1000;
}
"""

@window_manager_bp.route('/window_manager.js')
def serve_js():
    return Response(js_code, mimetype='application/javascript')
